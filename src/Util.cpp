//
// Created by maciej on 20.04.23.
//
#include "../include/Common.h"

void drawSpeedLimitOnFrame(const cv::Mat &frame, int speedLimitValue)  {
    std::stringstream speedLimitText;
    speedLimitText << "speed limit: " << speedLimitValue;
    cv::putText(frame, // target image
                speedLimitText.str(), // text
                cv::Point(10, frame.rows / 5), // top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                1.0,
                cv::Scalar(255, 0, 255), // font color
                2);
}