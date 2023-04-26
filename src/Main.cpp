
#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

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
    while (true) {
        if (!cap.read(frame)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        cv::waitKey(15); // change if calculation is too fast/slow

        auto *sign = (SpeedLimitSign *) detector.detectRoadSign(frame);
        if (sign->getLimit() != 0)
            lastSeenSign = sign;

        drawSpeedLimitOnFrame(frame, lastSeenSign->getLimit());

        if (DEBUG_MODE)
            std::cout << sign->toString() << std::endl;

        cv::imshow("Preview", frame);
    }
}
