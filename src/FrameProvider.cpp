#include "../include/FrameProvider.h"



FrameProvider::FrameProvider(const std::string &source, int bufferSize, FrameSourceType sourceType)
        : stopFlag(false), cap(source), frameBuffer(bufferSize), semaphore(bufferSize) {

    if (sourceType == FrameSourceType::VideoFile) {
        cap.open(source);
        if (!cap.isOpened()) {
            std::cerr << "Error opening video source" << std::endl;
        }
    }
    else if (sourceType == FrameSourceType::LiveCamera) {
        int cameraIndex = std::stoi(source);
        cap.open(cameraIndex);
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera with index: " << cameraIndex << std::endl;
        }
    }

    readThread = std::thread(&FrameProvider::readFrames, this);
}

FrameProvider::~FrameProvider() {
    stopFlag = true;
    readThread.detach();
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
