#pragma once

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../include/CircularBuffer.h"


class FrameProvider {
public:
    explicit FrameProvider(const std::string &source);

    ~FrameProvider();

    cv::Mat getFrame();

private:
    void readFrames();

    std::atomic<bool> stopFlag;
    CircularBuffer<cv::Mat> frameBuffer;
    std::mutex bufferMutex;
    std::condition_variable frameBufferNotEmpty;
    std::thread readThread;
    cv::VideoCapture cap;

    std::counting_semaphore<3> semaphore;
};

