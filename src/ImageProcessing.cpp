#include "../include/Common.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../include/ImageProcessing.h"


cv::Mat ImageProcessing::extractRedColorFromImage(const cv::Mat &hsvFrame) {
    static cv::Scalar lower_red1(0, 50, 30);
    static cv::Scalar upper_red1(8, 255, 255);
    static cv::Scalar lower_red2(160, 50, 30);
    static cv::Scalar upper_red2(180, 255, 255);
    static cv::Scalar lower_red_pink(138, 30, 60);
    static cv::Scalar upper_red_pink(179, 255, 250);
    static cv::Scalar lower_red_dark(0, 5, 0);
    static cv::Scalar upper_red_dark(10, 90, 50);

    cv::Mat red_mask, red_mask1, red_mask2, red_mask_pink, red_mask_claret, red_mask_dark;
    cv::inRange(hsvFrame, lower_red1, upper_red1, red_mask1);
    cv::inRange(hsvFrame, lower_red2, upper_red2, red_mask2);
    cv::inRange(hsvFrame, lower_red_pink, upper_red_pink, red_mask_pink);
    cv::inRange(hsvFrame, lower_red_dark, upper_red_dark, red_mask_dark);
    cv::bitwise_or(red_mask1, red_mask2, red_mask);
    cv::bitwise_or(red_mask, red_mask_pink, red_mask);
    cv::bitwise_or(red_mask, red_mask_dark, red_mask);

    return red_mask;
}

void ImageProcessing::blurImage(cv::Mat &image, int size) {
    cv::medianBlur(image, image, size);
}

void ImageProcessing::preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask) {
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

cv::Mat ImageProcessing::rawPreprocess(cv::Mat roi) {
    return roi;
}

cv::Mat ImageProcessing::binaryBrightPreprocess(cv::Mat roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::threshold(roi, roi, 200, 255, cv::THRESH_BINARY);

    return roi;
}

cv::Mat ImageProcessing::binaryDarkPreprocess(cv::Mat roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::threshold(roi, roi, 100, 255, cv::THRESH_BINARY);

    return roi;
}

cv::Mat ImageProcessing::adaptiveBrightnessPreprocess(cv::Mat roi) {
    double alpha = 1.5;  // contrast control, adjust as needed
    int beta = 20;       // brightness control, adjust as needed
    cv::Mat result;
    roi.convertTo(result, -1, alpha, beta);

    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(result, result);
    cv::bitwise_not(result, result);

    cv::adaptiveThreshold(result, result, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 81, 0);

    cv::erode(result, result, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::dilate(result, result, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    return result;
}
