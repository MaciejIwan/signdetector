#include "../include/IRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/baseapi.h>
#include "../include/common.h"
const int MAX_SIGN_LABELS = 100;

const float MIN_VALID_ACCURACY_RATE = 0.6;

const bool TEST_FALSE_POSITIVES = false;
const float MAX_VALID_FALSE_POSITIVES_RATE = 0.05;


struct SignLabel {
    int frameStart;
    int frameEnd;
    int speedSignValue;
};

int loadSignLabelsFromCSV(std::string filename, SignLabel *labels, int *labelCount) {
    using namespace std;

    if (!labels || !filename.length()) {
        cout << "No CSV file required for test" << endl;
        return 1;
    }

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    } else {
        cout << "Could not open label file\n";
        return 2;
    }

    for (int i = 0; i < content.size(); i++) {
        if (content[i].size() != 3) {
            cout << "Incorrect CSV file format at line " << i << endl;
            return 3;
        }

        labels[i].frameStart = stoi(content[i][0]);
        labels[i].frameEnd = stoi(content[i][1]);
        labels[i].speedSignValue = stoi(content[i][2]);
    }

    *labelCount = content.size();

    return 0;
}

int countTotalFramesWithSigns(SignLabel *labels, int labelCount) {
    int signFrames = 0;

    for (int i = 0; i < labelCount; i++) {
        signFrames += labels[i].frameEnd - labels[i].frameStart;
    }
    return signFrames;
}

TEST(VideoTest, SignRecognitionAccuracy) {
    std::string filename = "speed_limit_11";
    std::string videoFile = "video/" + filename + ".mp4";

    std::cout << "[ FILENAME ] " << videoFile << std::endl;

    // Load labels
    SignLabel labels[MAX_SIGN_LABELS];
    int labelCount = 0;

    // TODO: Make the CSV filepath dynamic
    int csvCheck = loadSignLabelsFromCSV("video/labels/" + filename + ".csv", labels, &labelCount);
    if (csvCheck) {
        return;
    }


    int framesWithSignCorrectlyRecognized = 0;
    int framesWithSignIncorrectlyRecognized = 0;

    int framesWithoutSignCorrectlyRecognized = 0;
    int framesWithoutSignIncorrectlyRecognized = 0;

    auto signDetector = ShapeRoadSignDetector();

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return;
    }

    int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

    cv::Mat frame;

    if(DEBUG_MODE)
        cv::namedWindow("Preview", cv::WINDOW_NORMAL);
    while (cap.read(frame)) {

        auto *sign = (SpeedLimitSign *) signDetector.detectRoadSign(frame);


        if (DEBUG_MODE) {
            //std::cout << sign->getLimit() << std::endl;
            cv::imshow("Preview", frame);
            cv::waitKey(1);
        }
        int currentFrameNumber = cap.get(cv::CAP_PROP_POS_FRAMES);


        bool frameCounted = false;

        // Current frame should contain the sign
        for (int i = 0; i < labelCount; i++) {
            if (currentFrameNumber >= labels[i].frameStart && currentFrameNumber <= labels[i].frameEnd) {
                if (sign->getLimit() == labels[i].speedSignValue) {
                    framesWithSignCorrectlyRecognized++;
                } else {
                    framesWithSignIncorrectlyRecognized++;
                }
                frameCounted = true;
                break;
            }
        }

        if (!frameCounted) {
            // Current frame should not contain any sign
            if (sign->getLimit() == 0) {
                framesWithoutSignCorrectlyRecognized++;
            } else {
                framesWithoutSignIncorrectlyRecognized++;
            }
        }
    }


    EXPECT_EQ(
            framesWithSignCorrectlyRecognized + framesWithSignIncorrectlyRecognized +
            framesWithoutSignCorrectlyRecognized + framesWithoutSignIncorrectlyRecognized, totalFrames);

    const int totalSignFrames = countTotalFramesWithSigns(labels, labelCount);
    const int totalNoSignFrames = totalFrames - totalSignFrames;

    float signRecognizedAccuracy = framesWithSignCorrectlyRecognized / (float) totalSignFrames;
    float falsePositivesAccuracy = framesWithoutSignIncorrectlyRecognized / (float) totalNoSignFrames;

    std::cout << "[ ACCURACY ] " << signRecognizedAccuracy * 100 << "%" << std::endl;
    std::cout << "[FALSE POS.] " << falsePositivesAccuracy * 100 << "%" << std::endl;

    EXPECT_TRUE(signRecognizedAccuracy >= MIN_VALID_ACCURACY_RATE);

    if (TEST_FALSE_POSITIVES)
        EXPECT_TRUE(falsePositivesAccuracy <= MAX_VALID_FALSE_POSITIVES_RATE);
}
