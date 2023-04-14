//
// Created by maciej on 14.04.23.
//

#include <regex>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include "../include/IRoadSignDetector.h"
#include "../include/models/RoadSign.h"


ShapeRoadSignDetector::ShapeRoadSignDetector() : myOcr() {

}


ShapeRoadSignDetector::~ShapeRoadSignDetector() = default;

RoadSign ShapeRoadSignDetector::detectRoadSigns(const cv::Mat &image) {
    RoadSign roadSign = RoadSign(SignType::SPEED_LIMIT, 30);


    return roadSign;
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
    static cv::Scalar upper_red1(15, 255, 255);
    static cv::Scalar lower_red2(160, 50, 30);
    static cv::Scalar upper_red2(180, 255, 255);
    static cv::Scalar lower_red_pink(140, 50, 50);
    static cv::Scalar upper_red_pink(160, 255, 255);
    static cv::Scalar lower_red_claret(0, 50, 10);
    static cv::Scalar upper_red_claret(10, 255, 150);
    static cv::Scalar lower_red_dark(0, 5, 0);
    static cv::Scalar upper_red_dark(10, 90, 50);

    cv::Mat red_mask, red_mask1, red_mask2, red_mask_pink, red_mask_claret, red_mask_dark;
    cv::inRange(hsvFrame, lower_red1, upper_red1, red_mask1);
    cv::inRange(hsvFrame, lower_red2, upper_red2, red_mask2);
    cv::inRange(hsvFrame, lower_red_pink, upper_red_pink, red_mask_pink);
    cv::inRange(hsvFrame, lower_red_claret, upper_red_claret, red_mask_claret);
    cv::inRange(hsvFrame, lower_red_dark, upper_red_dark, red_mask_dark);
    cv::bitwise_or(red_mask1, red_mask2, red_mask);
    cv::bitwise_or(red_mask, red_mask_pink, red_mask);
    cv::bitwise_or(red_mask, red_mask_claret, red_mask);
    cv::bitwise_or(red_mask, red_mask_dark, red_mask);

    return red_mask;
}


void ShapeRoadSignDetector::updateImageView(cv::Mat &currentFrame, int &lastSpeedLimit) {
    blurImage(currentFrame, 5);

    cv::Mat hsvFrame;
    cv::cvtColor(currentFrame, hsvFrame, cv::COLOR_BGR2HSV);


    cv::Mat red_binary_mask = extractRedColorFromImage(hsvFrame);

    // Zdefiniuj rozmiar jądra dla operacji morfologicznych
    cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    // Zastosuj operację otwarcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_OPEN, kernel);
    // Zastosuj operację zamknięcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_CLOSE, kernel);

    cv::Mat maskedFrame;
    currentFrame.copyTo(maskedFrame, red_binary_mask);

    // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_binary_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat contourPreview = currentFrame.clone();
    cv::Mat temp = currentFrame.clone(); // frame for tesseract

    // Iterate through each contour and check if it is a circle
    for (const auto &contour: contours) {
        double area = cv::contourArea(contour);
        if (area > 750) {
            cv::Rect bounding_rect = cv::boundingRect(contour);
            double contour_similarity = compareContoursToCircle(contour);
            if (contour_similarity >= MIN_CONTOUR_SIMILARITY) {
                cv::Mat roi = temp(bounding_rect); // Extract the ROI from the current frame
                int numberFromRoi = myOcr.getNumberFromRoi(roi);
                if (numberFromRoi != 0) {
                    lastSpeedLimit = numberFromRoi;
                }
                cv::rectangle(currentFrame, bounding_rect, cv::Scalar(0, 255, 0), 2);

            }
            cv::drawContours(contourPreview, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 255, 0),
                             2);
            std::stringstream similarity_text;
            similarity_text << "Similarity: " << std::fixed << std::setprecision(2) << contour_similarity;
            cv::putText(contourPreview, similarity_text.str(),
                        cv::Point(bounding_rect.x, bounding_rect.y + bounding_rect.height + 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
    }
    std::stringstream sppedLimitText;
    sppedLimitText << "speed limit: " << lastSpeedLimit;
    cv::putText(currentFrame, //target image
                sppedLimitText.str(), //text
                cv::Point(10, currentFrame.rows / 5), //top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                1.0,
                cv::Scalar(255, 0, 255), //font color
                2);

    // Display original and masked images, as well as contour preview
    cv::imshow("Preview", currentFrame);
    cv::imshow("Masked Image", maskedFrame);
    cv::imshow("red color mask", red_binary_mask);
    cv::imshow("Contour Preview", contourPreview);

}
