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

        flipImage = true;
    }

    readThread = std::thread(&FrameProvider::readFrames, this);
}

FrameProvider::~FrameProvider() {
    stopFlag = true;

    //semaphore.release();
    frameBufferNotEmpty.notify_one();

    if (readThread.joinable()) {
        readThread.join();
    }

    cap.release();
    std::cout << "FrameProvider deleted" << std::endl;
}

cv::Mat FrameProvider::getFrame() {
    if(videoFinished) //todo it should check if buffer is empty
        throw std::exception();
    std::unique_lock<std::mutex> lock(bufferMutex);
    frameBufferNotEmpty.wait(lock, [this] { return !frameBuffer.isEmpty(); });

    cv::Mat frame = frameBuffer.pop();
    semaphore.release();

    if(flipImage)
        cv::flip(frame, frame, -1);
    return frame;
}

void FrameProvider::readFrames() {
    while (!stopFlag) {
        cv::Mat frame;
        if (!cap.read(frame)) {
            videoFinished = true;
            break;
        }

        semaphore.acquire();
        if(stopFlag){
            break;
        }

        bufferMutex.lock();
        frameBuffer.push(frame);
        bufferMutex.unlock();

        frameBufferNotEmpty.notify_one();


    }
}
