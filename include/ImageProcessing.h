//
// Created by maciej on 14.04.23.
//

#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/opencv.hpp>
#include <regex>
#include <tesseract/baseapi.h>

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

double compareContoursToCircle(const std::vector<cv::Point>& contour);

void blurImage(cv::Mat& image, int size, double sigma);
int getNumberFromRoi(const cv::Mat& roi, tesseract::TessBaseAPI* ocr);

void updateImageView(cv::Mat& currentFrame, tesseract::TessBaseAPI* ocr, int& lastSpeedLimit);
cv::Mat extractRedColorFromImage(const cv::Mat& hsvFrame);

#endif
