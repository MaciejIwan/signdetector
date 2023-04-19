
#include "../include/IRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

int main(int argc, char** argv)
{
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    ShapeRoadSignDetector detector = ShapeRoadSignDetector();
    int lastSpeedLimit = 0;

    std::string videoFile = "video/znak2.mp4";
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

        cv::waitKey(20); // change if calculation is too fast/slow


        auto* sign = (SpeedLimitSign*)detector.detectRoadSign(frame);
        std::cout << sign->toString() << std::endl;

        cv::imshow("Preview", frame);
    }
}
