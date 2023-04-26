//
// Created by maciej on 19.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_COMMON_H
#define SIGN_EDGE_METHOD_TEST_1_COMMON_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>



// DEBUGGING SETUP

// main switch for debugging
#define DEBUG_MODE true

// true to see OCR image && print raw OCR text
#define DEBUG_OCR_IMG false

// how long u see OCR IMG before next frame.
//0 wait for key press, 10 wait for 10ms
#define DEBUG_OCR_IMG_DELAY 1

#define DEBUG_OCR_CONSOLE_LOG false

void drawSpeedLimitOnFrame(const cv::Mat &frame, int speedLimitValue);

#endif //SIGN_EDGE_METHOD_TEST_1_COMMON_H
