#include "../include/utils/VideoTestUtils.h"
#include "../../include/IRoadSignDetector.h"
#include "../../include/ShapeRoadSignDetector.h"
#include "../../include/Common.h"
#include "../../include/models/RoadSign.h"
#include "../../include/models/SpeedLimitSign.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/baseapi.h>


const int MAX_SIGN_LABELS = 100;

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
        labels[i].detected = false;
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

int countDetectedSigns(SignLabel *labels, int labelCount) {
    int detectedSigns = 0;

    for (int i = 0; i < labelCount; i++) {
        if (labels[i].detected) {
            detectedSigns++;
        }
    }
    return detectedSigns;
}

void testSignRecognitionAccuracy(const std::string &filename) {
    std::string videoFile = "video/" + filename + ".mp4";



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

    if (DEBUG_MODE)
        cv::namedWindow("Preview", cv::WINDOW_NORMAL);
    auto start = std::chrono::steady_clock::now();
    while (cap.read(frame)) {
        auto *sign = (SpeedLimitSign *) signDetector.detectRoadSign(frame);

        if (DEBUG_MODE) {
            drawSpeedLimitOnFrame(frame, sign->getLimit(), 0.0);
            cv::imshow("Preview", frame);
            cv::waitKey(delay);
        }

        int currentFrameNumber = cap.get(cv::CAP_PROP_POS_FRAMES);
        bool frameCounted = false;

        // Current frame should contain the sign
        for (int i = 0; i < labelCount; i++) {
            if (currentFrameNumber >= labels[i].frameStart && currentFrameNumber <= labels[i].frameEnd) {
                if (sign->getLimit() == labels[i].speedSignValue) {
                    framesWithSignCorrectlyRecognized++;
                    labels[i].detected = true;
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
        delete sign;
    }

    EXPECT_EQ(framesWithSignCorrectlyRecognized + framesWithSignIncorrectlyRecognized +
              framesWithoutSignCorrectlyRecognized + framesWithoutSignIncorrectlyRecognized, totalFrames);

    const int totalSignFrames = countTotalFramesWithSigns(labels, labelCount);
    const int totalNoSignFrames = totalFrames - totalSignFrames;

    float signRecognizedAccuracy = framesWithSignCorrectlyRecognized / (float) totalSignFrames;
    float falsePositivesAccuracy = framesWithoutSignIncorrectlyRecognized / (float) totalNoSignFrames;

    int detectedSigns = countDetectedSigns(labels, labelCount);


    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;


    std::cout << "[\tFILENAME\t]\t" << videoFile << std::endl;
    std::cout << "[\tDETECTED\t]\t" << detectedSigns << " out of " << labelCount << " signs." << std::endl;
    std::cout << "[\tACCURACY\t]\t" << signRecognizedAccuracy * 100 << "%" << std::endl;
    std::cout << "[\tEXEC TIME\t]\t" << elapsedSeconds.count() << " seconds" << std::endl;
    //std::cout << "[ FALSE POS. ] " << falsePositivesAccuracy * 100 << "%" << std::endl;


}
