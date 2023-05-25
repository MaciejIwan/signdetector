#pragma once

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "CircularBuffer.h"

std::counting_semaphore<3> semaphore(3);

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
};

FrameProvider::FrameProvider(const std::string &source)
        : stopFlag(false), cap(source), frameBuffer(3) {
    if (!cap.isOpened()) {
        std::cerr << "Error opening video source" << std::endl;
    }

    readThread = std::thread(&FrameProvider::readFrames, this);
}

FrameProvider::~FrameProvider() {
    stopFlag = true;

    if (readThread.joinable()) {
        readThread.join();
    }
}

cv::Mat FrameProvider::getFrame() {
    std::unique_lock<std::mutex> lock(bufferMutex);
    frameBufferNotEmpty.wait(lock, [this] { return !frameBuffer.isEmpty(); });

    cv::Mat frame = frameBuffer.pop();
    semaphore.release();

    return frame;
}

void FrameProvider::readFrames() {
    while (!stopFlag) {

        cv::Mat frame;
        if (!cap.read(frame)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }
        semaphore.acquire();

        bufferMutex.lock();
        frameBuffer.push(frame);
        bufferMutex.unlock();

        frameBufferNotEmpty.notify_one();
    }
}
