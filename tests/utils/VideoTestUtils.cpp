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



void VideoTest::testSignRecognitionAccuracy(const std::string& filename) {
    this->filename = filename;
    loadFiles();
    auto signDetector = ShapeRoadSignDetector();

    if (DEBUG_MODE)
        cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    while (cap.read(frame)) {
        auto* sign = (SpeedLimitSign*)signDetector.detectRoadSign(frame);

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

    calculateStatistics();
    int framesSum = framesWithSignCorrectlyRecognized + framesWithSignIncorrectlyRecognized + framesWithoutSignCorrectlyRecognized + framesWithoutSignIncorrectlyRecognized;

    EXPECT_EQ(framesSum, totalFrames);
    EXPECT_TRUE(detectedSigns == labelCount);
}

void VideoTest::calculateStatistics() {
    const int totalSignFrames = countTotalFramesWithSigns(labels, labelCount);
    const int totalNoSignFrames = totalFrames - totalSignFrames;

    signRecognizedAccuracy = framesWithSignCorrectlyRecognized / (float)totalSignFrames;
    float falsePositivesAccuracy = framesWithoutSignIncorrectlyRecognized / (float)totalNoSignFrames;

    detectedSigns = countDetectedSigns(labels, labelCount);

    auto end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;
}

void VideoTest::SetUp() {
    start = std::chrono::steady_clock::now();
}

void VideoTest::TearDown() {
    //std::ofstream reportFile("report.txt", std::ios::trunc);
    std::ofstream reportFile("report.txt", std::ios::app);

    std::cout << "[\tFILENAME\t]\t" << filename << std::endl;
    std::cout << "[\tDETECTED\t]\t" << detectedSigns << " out of " << labelCount << " signs." << std::endl;
    std::cout << "[\tACCURACY\t]\t" << signRecognizedAccuracy * 100 << "%" << std::endl;
    std::cout << "[\tEXEC TIME\t]\t" << elapsedSeconds.count() << " seconds" << std::endl;

    reportFile << filename << "\t";
    reportFile << detectedSigns << " of " << labelCount << "\t";
    reportFile << signRecognizedAccuracy * 100 << "%" << "\t";
    reportFile << elapsedSeconds.count() << " seconds";

    reportFile << std::endl;
    reportFile.close();
    cap.release();
}

void VideoTest::loadFiles() {
    std::string videoFile = "video/" + filename + ".mp4";

    labelCount = 0;

    int csvCheck = loadSignLabelsFromCSV("video/labels/" + filename + ".csv", labels, &labelCount);
    if (csvCheck) {
        return;
    }

    cv::VideoCapture cap(videoFile);
    this->cap = cap;

    totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return;
    }
}

int VideoTest::countTotalFramesWithSigns(SignLabel* labels, int labelCount) {
    int signFrames = 0;

    for (int i = 0; i < labelCount; i++) {
        signFrames += labels[i].frameEnd - labels[i].frameStart;
    }
    return signFrames;
}

int VideoTest::countDetectedSigns(SignLabel* labels, int labelCount) {
    int detectedSigns = 0;

    for (int i = 0; i < labelCount; i++) {
        if (labels[i].detected) {
            detectedSigns++;
        }
    }
    return detectedSigns;
}

int VideoTest::loadSignLabelsFromCSV(std::string filename, SignLabel* labels, int* labelCount) {
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
