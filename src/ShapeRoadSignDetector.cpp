//
// Created by maciej on 14.04.23.
//

// #include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"

#include <iomanip>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "../include/models/SpeedLimitSign.h"

ShapeRoadSignDetector::ShapeRoadSignDetector()
    : ocr() {
}

ShapeRoadSignDetector::~ShapeRoadSignDetector() = default;

void extractWhiteColorFromImage(const cv::Mat &currentFrame, cv::Mat &white_binary_mask) {
    cv::Mat grayImage;
    cv::cvtColor(currentFrame, grayImage, cv::COLOR_BGR2GRAY);

    // Apply thresholding to the gray image to isolate white pixels
    cv::Mat thresholdedImage;
    cv::threshold(grayImage, thresholdedImage, 200, 255, cv::THRESH_BINARY);

    // Apply a morphological opening operation to remove small noise
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(thresholdedImage, thresholdedImage, cv::MORPH_OPEN, kernel);

    if (DEBUG_MODE) {
        cv::imshow("Masked Image White", thresholdedImage);
    }

    white_binary_mask = thresholdedImage;
}


RoadSign *ShapeRoadSignDetector::detectRoadSign(cv::Mat &image) {
    cv::Mat red_binary_mask;
    cv::Mat white_binary_mask;
    preprocess(image, red_binary_mask);
    extractWhiteColorFromImage(image, white_binary_mask);

    std::vector<cv::Rect> boundingBoxes;
    findSignsBoundingBoxes(red_binary_mask, boundingBoxes);

    cv::Mat cached_image = image.clone();
    int lastSpeedLimit = 0;
    for (cv::Rect bounding_rect : boundingBoxes) {
        cv::Mat roi = cached_image(bounding_rect);

        int numberFromRoi = ocr.getNumberFromRoi(roi);
        if (numberFromRoi != 0) {
            lastSpeedLimit = numberFromRoi;
        }
        cv::rectangle(image, bounding_rect, cv::Scalar(0, 255, 0), 2);
    }

    return new SpeedLimitSign(lastSpeedLimit);
}

void ShapeRoadSignDetector::blurImage(cv::Mat &image, int size) {
    cv::medianBlur(image, image, size);
}

double ShapeRoadSignDetector::compareContoursToCircle(const std::vector<cv::Point> &contour) {
    // Fit a minimum enclosing circle to the contour
    cv::Point2f center;
    float radius;
    cv::minEnclosingCircle(contour, center, radius);

    // Calculate the area of the contour and the area of the enclosing circle
    double contour_area = cv::contourArea(contour);
    double circle_area = CV_PI * radius * radius;

    // Calculate the similarity ratio between the contour and the circle
    double similarity = contour_area / circle_area;

    return similarity;
}

cv::Mat ShapeRoadSignDetector::extractRedColorFromImage(const cv::Mat &hsvFrame) {
    static cv::Scalar lower_red1(0, 50, 30);
    static cv::Scalar upper_red1(8, 255, 255);
    static cv::Scalar lower_red2(160, 50, 30);
    static cv::Scalar upper_red2(180, 255, 255);
    static cv::Scalar lower_red_pink(138, 30, 60);
    static cv::Scalar upper_red_pink(179, 255, 250);
    //    static cv::Scalar lower_red_claret(0, 50, 10);
    //    static cv::Scalar upper_red_claret(10, 255, 150);
    static cv::Scalar lower_red_dark(0, 5, 0);
    static cv::Scalar upper_red_dark(10, 90, 50);

    cv::Mat red_mask, red_mask1, red_mask2, red_mask_pink, red_mask_claret, red_mask_dark;
    cv::inRange(hsvFrame, lower_red1, upper_red1, red_mask1);
    cv::inRange(hsvFrame, lower_red2, upper_red2, red_mask2);
    cv::inRange(hsvFrame, lower_red_pink, upper_red_pink, red_mask_pink);
    // cv::inRange(hsvFrame, lower_red_claret, upper_red_claret, red_mask_claret);
    cv::inRange(hsvFrame, lower_red_dark, upper_red_dark, red_mask_dark);
    cv::bitwise_or(red_mask1, red_mask2, red_mask);
    cv::bitwise_or(red_mask, red_mask_pink, red_mask);
    // cv::bitwise_or(red_mask, red_mask_claret, red_mask);
    cv::bitwise_or(red_mask, red_mask_dark, red_mask);

    return red_mask;
}

void ShapeRoadSignDetector::findSignsBoundingBoxes(const cv::Mat &red_binary_mask,
                                                   std::vector<cv::Rect> &boundingBoxes) const {  // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_binary_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    // cv::Mat contourPreview = currentFrame.clone();

    // Iterate through each contour and check if it is a circle
    for (const auto &contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 750) {
            cv::Rect bounding_rect = cv::boundingRect(contour);
            double contour_similarity = compareContoursToCircle(contour);
            if (contour_similarity >= MIN_CONTOUR_SIMILARITY) {
                boundingBoxes.push_back(bounding_rect);
            }
        }
    }
}

void ShapeRoadSignDetector::preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask) {
    // blurImage(currentFrame, 3);

    cv::Mat hsvFrame;
    cv::cvtColor(currentFrame, hsvFrame, cv::COLOR_BGR2HSV);  // Zdefiniuj rozmiar jądra dla operacji morfologicznych
    red_binary_mask = extractRedColorFromImage(hsvFrame);

    cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    // Zastosuj operację otwarcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_OPEN, kernel);
    // Zastosuj operację zamknięcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_CLOSE, kernel);

    if (DEBUG_MODE) {
        cv::Mat maskedFrame;
        currentFrame.copyTo(maskedFrame, red_binary_mask);
        cv::imshow("Masked Image Red", maskedFrame);
    }
}
