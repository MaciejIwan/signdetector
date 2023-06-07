#include "../include/FrameProvider.h"



FrameProvider::FrameProvider(const FrameProviderConfig &config)
        : stopFlag(false), cap(config.framesSource), frameBuffer(config.bufferSize), semaphore(config.bufferSize) {

    if (config.sourceType == FrameSourceType::VideoFile) {
        cap.open(config.framesSource);
        if (!cap.isOpened()) {
            std::cerr << "Error opening video source" << std::endl;
        }
    }
    else if (config.sourceType == FrameSourceType::LiveCamera) {
        int cameraIndex = std::stoi(config.framesSource);
        cap.open(cameraIndex);
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera with index: " << cameraIndex << std::endl;
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, config.width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.height);

        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap.set(cv::CAP_PROP_FPS, config.frameRate);
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
            break;
        }
        semaphore.acquire();

        bufferMutex.lock();
        frameBuffer.push(frame);
        bufferMutex.unlock();

        frameBufferNotEmpty.notify_one();
    }
}
