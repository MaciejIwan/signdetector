#pragma once


class ImageProcessing {
public:
    static cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);
    static void blurImage(cv::Mat &image, int size);
    static void preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask);
    static cv::Mat adaptiveBrightnessPreprocess(cv::Mat roi);
    static cv::Mat binaryDarkPreprocess(cv::Mat roi);
    static cv::Mat binaryBrightPreprocess(cv::Mat roi);
    static cv::Mat rawPreprocess(cv::Mat roi);
};

