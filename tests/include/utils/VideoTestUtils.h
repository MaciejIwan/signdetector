#pragma once

#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <gtest/gtest.h>

const int MAX_SIGN_LABELS = 100;
static const int delay = 1;
struct SignLabel {
    int frameStart;
    int frameEnd;
    int speedSignValue;
    bool detected;
};


class VideoTest : public ::testing::Test {
public:
    void testSignRecognitionAccuracy(const std::string& filename);
    void calculateStatistics();

protected:
    void SetUp() override;
    void TearDown() override;
    void loadFiles();
    int countTotalFramesWithSigns(SignLabel* labels, int labelCount);
    int countDetectedSigns(SignLabel* labels, int labelCount);
    int loadSignLabelsFromCSV(std::string filename, SignLabel* labels, int* labelCount);

private:
    cv::Mat frame;
    std::chrono::steady_clock::time_point start;
    SignLabel labels[MAX_SIGN_LABELS];
    std::chrono::duration<double> elapsedSeconds;
    int totalFrames = 0;
    float signRecognizedAccuracy = 0.0;
    std::string filename;
    int labelCount = 0;
    int detectedSigns = 0;
    int framesWithSignCorrectlyRecognized = 0;
    int framesWithSignIncorrectlyRecognized = 0;
    int framesWithoutSignCorrectlyRecognized = 0;
    int framesWithoutSignIncorrectlyRecognized = 0;
    cv::VideoCapture cap;

    void writeLogToFile() const;
};
