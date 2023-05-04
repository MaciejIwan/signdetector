#include <chrono>
#include <iostream>
#include <gtest/gtest.h>
#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include <opencv2/opencv.hpp>

class RoadSignDetectorTest : public ::testing::Test {
protected:
    ShapeRoadSignDetector detector;
};

TEST_F(RoadSignDetectorTest, detectRoadSignPerformance) {
    cv::VideoCapture cap("video/speed_limit_seqence_1.mp4");

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return;
    }

    cv::Mat frame;
    int numFrames = 0;
    double fpsSum = 0.0;
    double minFps = std::numeric_limits<double>::max();
    double maxFps = std::numeric_limits<double>::min();

    auto startTime = std::chrono::steady_clock::now();

    while (cap.read(frame)) {
        auto startFrameTime = std::chrono::steady_clock::now();

        detector.detectRoadSign(frame);

        auto endFrameTime = std::chrono::steady_clock::now();
        double fps = 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(endFrameTime - startFrameTime).count();
        fpsSum += fps;
        minFps = std::min(minFps, fps);
        maxFps = std::max(maxFps, fps);
        numFrames++;
    }

    auto endTime = std::chrono::steady_clock::now();
    double totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    double avgFps = fpsSum / numFrames;

    std::cout << "Total time: " << totalTime << " ms" << std::endl;
    std::cout << "Number of frames: " << numFrames << std::endl;
    std::cout << "Average FPS: " << avgFps << std::endl;
    std::cout << "Minimum FPS: " << minFps << std::endl;
    std::cout << "Maximum FPS: " << maxFps << std::endl;

    ASSERT_GE(avgFps, 30) << "Average FPS is less than 30";
}
