#include "../include/ImageProcessing.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/baseapi.h>

const int MAX_SIGN_LABELS = 100;

const float MIN_VALID_ACCURACY_RATE = 0.9;

const bool TEST_FALSE_POSITIVES = false;
const float MAX_VALID_FALSE_POSITIVES_RATE = 0.05;

struct SignLabel {
    int frameStart;
    int frameEnd;
    int speedSignValue;
};

int loadSignLabelsFromCSV(std::string filename, SignLabel* labels, int* labelCount)
{
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

int countTotalFramesWithSigns(SignLabel* labels, int labelCount)
{
    int signFrames = 0;

    for (int i = 0; i < labelCount; i++) {
        signFrames += labels[i].frameEnd - labels[i].frameStart;
    }
    return signFrames;
}

TEST(VideoTest, SignRecognitionAccuracy)
{
    std::string videoFile = "video/speed_limit_0.mp4";

    std::cout << "[ FILENAME ] " << videoFile << std::endl;

    // Load labels
    SignLabel labels[MAX_SIGN_LABELS];
    int labelCount = 0;

    // TODO: Make the CSV filepath dynamic
    int csvCheck = loadSignLabelsFromCSV("video/labels/speed_limit_0.csv", labels, &labelCount);
    if (csvCheck) {
        return;
    }

    int lastSpeedLimit = 0;

    int framesWithSignCorrectlyRecignized = 0;
    int framesWithSignIncorrectlyRecignized = 0;

    int framesWithoutSignCorrectlyRecignized = 0;
    int framesWithoutSignIncorrectlyRecignized = 0;

    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return;
    }

    int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

    cv::Mat frame;

    while (cap.read(frame)) {
        updateImageView(frame, ocr, lastSpeedLimit);

        int currentFrame = cap.get(cv::CAP_PROP_POS_FRAMES);

        bool frameCounted = false;

        // Current frame should contain the sign
        for (int i = 0; i < labelCount; i++) {
            if (currentFrame >= labels[i].frameStart && currentFrame <= labels[i].frameEnd) {
                if (lastSpeedLimit == labels[i].speedSignValue) {
                    framesWithSignCorrectlyRecignized++;
                } else {
                    framesWithSignIncorrectlyRecignized++;
                }
                frameCounted = true;
                break;
            }
        }

        if (!frameCounted) {
            // Current frame should not contain any sign
            if (lastSpeedLimit == 0) {
                framesWithoutSignCorrectlyRecignized++;
            } else {
                framesWithoutSignIncorrectlyRecignized++;
            }
        }
    }

    ocr->End();
    delete ocr;

    EXPECT_EQ(
        framesWithSignCorrectlyRecignized + framesWithSignIncorrectlyRecignized + framesWithoutSignCorrectlyRecignized + framesWithoutSignIncorrectlyRecignized, totalFrames);

    const int totalSignFrames = countTotalFramesWithSigns(labels, labelCount);
    const int titalNoSignFrames = totalFrames - totalSignFrames;

    float signRecognizedAccuracy = framesWithSignCorrectlyRecignized / (float)totalSignFrames;
    float falsePositivesAccuracy = framesWithoutSignIncorrectlyRecignized / (float)titalNoSignFrames;

    std::cout << "[ ACCURACY ] " << signRecognizedAccuracy * 100 << "%" << std::endl;
    std::cout << "[FALSE POS.] " << falsePositivesAccuracy * 100 << "%" << std::endl;

    EXPECT_TRUE(signRecognizedAccuracy >= MIN_VALID_ACCURACY_RATE);

    if (TEST_FALSE_POSITIVES)
        EXPECT_TRUE(falsePositivesAccuracy <= MAX_VALID_FALSE_POSITIVES_RATE);
}
