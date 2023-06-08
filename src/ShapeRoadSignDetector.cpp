#include "../include/ShapeRoadSignDetector.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "../include/Common.h"
#include "../include/models/SpeedLimitSign.h"
#include "../include/ImageProcessing.h"

ShapeRoadSignDetector::ShapeRoadSignDetector()
    : ocr() {
}

ShapeRoadSignDetector::~ShapeRoadSignDetector() = default;

RoadSign *ShapeRoadSignDetector::detectRoadSign(cv::Mat &image) {
    SpeedLimitSign speedLimitSign(0);
    cv::Mat red_binary_mask;
    ImageProcessing::preprocess(image, red_binary_mask);

    std::vector<cv::Rect> boundingBoxes;
    findSignsBoundingBoxes(red_binary_mask, boundingBoxes);

    cv::Mat cached_image = image.clone();
    int lastSpeedLimit = 0;
    for (cv::Rect bounding_rect : boundingBoxes) {
        cv::Mat roi = cached_image(bounding_rect);
        if (DEBUG_OCR_IMG && DEBUG_MODE) {
            cv::imshow("roi", roi);
        }

        for (const auto &preprocessFunction : ocr.getpreprocessVector()) {
            speedLimitSign = SpeedLimitSign(ocr.getNumberFromRoi(roi, preprocessFunction));
            if (speedLimitSign.getLimit() == SpeedLimitSign::DEFAULT_SPEED_LIMIT) {
                continue;
            } else {
                break;
            }
        }
        if(DEBUG_MODE)
            cv::rectangle(image, bounding_rect, cv::Scalar(0, 255, 0), 2);
    }

    return new SpeedLimitSign(speedLimitSign);
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

cv::Rect findMaxSizeBoxInContour(const std::vector<cv::Point> &contour, const cv::Size &imageSize) {
    cv::RotatedRect minRect = cv::minAreaRect(contour);
    float rectWidth = minRect.size.width;
    float rectHeight = minRect.size.height;
    float sideLength = std::min(rectWidth, rectHeight);
    cv::Point2f center = minRect.center;
    cv::Point2f topLeft(center.x - sideLength / 2, center.y - sideLength / 2);
    cv::Rect squareRect(topLeft, cv::Size(sideLength, sideLength));
    squareRect &= cv::Rect(cv::Point(), imageSize);

    return squareRect;
}

void ShapeRoadSignDetector::findSignsBoundingBoxes(const cv::Mat &red_binary_mask,
                                                   std::vector<cv::Rect> &boundingBoxes) const {  // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_binary_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &contour : contours) {
        double area = cv::contourArea(contour);
        if (area < 750)
            continue;

        double contour_similarity = compareContoursToCircle(contour);
        if (contour_similarity >= MIN_CONTOUR_SIMILARITY) {
            cv::Rect maxSquareBox = findMaxSizeBoxInContour(contour, red_binary_mask.size());
            boundingBoxes.push_back(maxSquareBox);
        }
    }
}

