#include "../include/Common.h"
#include <iomanip>

void drawSpeedLimitOnFrame(const cv::Mat &frame, int speedLimitValue, double fps) {
    std::stringstream speedLimitText;
    //speedLimitText << "Speed Limit: " << speedLimitValue;

    std::stringstream fpsText;
    fpsText << "FPS: " << std::setprecision(2) << std::fixed << fps;

    cv::putText(frame, // target image
                speedLimitText.str(), // text
                cv::Point(10, frame.rows / 5), // top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                1.0,
                cv::Scalar(255, 0, 255), // font color
                2);

    cv::putText(frame, // target image
                fpsText.str(), // text
                cv::Point(10, frame.rows / 5 + 50), // top-left position (below speed)
                cv::FONT_HERSHEY_SIMPLEX,
                1.0,
                cv::Scalar(255, 0, 255), // font color
                2);
}
