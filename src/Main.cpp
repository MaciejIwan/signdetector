#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include "../include/CircularBuffer.h"

int main(int argc, char **argv) {
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    ShapeRoadSignDetector detector = ShapeRoadSignDetector();
    auto *lastSeenSign = new SpeedLimitSign(SpeedLimitSign::DEFAULT_SPEED_LIMIT);

    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return -1;
    }

    cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    cv::Mat frame;
    int64 prevTickCount = cv::getTickCount();
    double fps = 0;

    // TODO: it should keep Signs not ints
    CircularBuffer<int> buffer(30);
    while (true) {
        if (!cap.read(frame)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        int64 curTickCount = cv::getTickCount();
        double timeElapsed = (curTickCount - prevTickCount) / cv::getTickFrequency();
        prevTickCount = curTickCount;
        fps = 1 / timeElapsed;

        auto *sign = (SpeedLimitSign *) detector.detectRoadSign(frame);

        // todo integrate buffer with system (maybe decetor module). Now it is just print value to console
        buffer.push(sign->getLimit());
        int mostPopular = buffer.findMostPopularValue();
        if (mostPopular != 0) {
            std::cout << "mostPopular" << mostPopular << std::endl;
            if (sign->getLimit() != lastSeenSign->getLimit()) {
                std::cout << "Speed limit changed from " << lastSeenSign->getLimit() << " to " << sign->getLimit()
                          << std::endl;
                lastSeenSign = sign;
            }
        }
        if (sign->getLimit() != 0) {
            std::cout << *sign << std::endl;
        }

        if (sign->getLimit() != 0)
            lastSeenSign = sign;

        drawSpeedLimitOnFrame(frame, lastSeenSign->getLimit(), fps);

        cv::imshow("Preview", frame);
        cv::waitKey(15); // change if calculation is too fast/slow
    }
    delete lastSeenSign;
}


