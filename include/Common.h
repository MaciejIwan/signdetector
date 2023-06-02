#pragma once

#include <opencv2/core/mat.hpp>

// DEBUGGING SETUP

// main switch for debugging
#define DEBUG_MODE false

// true to see OCR image && print raw OCR text
#define DEBUG_OCR_IMG false

// how long u see OCR IMG before next frame.
// 0 wait for key press, 10 wait for 10ms
// do not how impact if DEBUG_OCR_IMG is false
#define DEBUG_OCR_IMG_DELAY 0

#define DEBUG_OCR_CONSOLE_LOG false

void drawSpeedLimitOnFrame(const cv::Mat &frame, int speedLimitValue, double fps);
