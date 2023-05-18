#include <iostream>
#include <opencv2/opencv.hpp>


// webpage to pick colors
// https://colorpicker.me/#ffbb87


std::tuple<int, int, int> fixHSVRange(int h, int s, int v) {
    // Normal H,S,V: (0-360,0-100%,0-100%)
    // OpenCV H,S,V: (0-180,0-255 ,0-255)
    int fixedH = 180 * h / 360;
    int fixedS = 255 * s / 100;
    int fixedV = 255 * v / 100;
    return std::make_tuple(fixedH, fixedS, fixedV);
}

int main() {
    // Load the input image in the HSV color space
    cv::Mat inputImage = cv::imread("img/gradient3.png", cv::IMREAD_COLOR);
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    // Define the lower and upper bounds of the range of HSV values to extract
    cv::Scalar lowerb(0, 90, 0);
    cv::Scalar upperb(18, 255, 255);

    cv::Scalar lowerb2(150, 108, 0);
    cv::Scalar upperb2(180, 255, 255);


    cv::Mat mask, mask1, mask2;
    cv::inRange(hsvImage, lowerb, upperb, mask1);
    cv::inRange(hsvImage, lowerb2, upperb2, mask2);
    cv::bitwise_or(mask1, mask2, mask);


    cv::imshow("input Image", inputImage);
    cv::imshow("hsv Image", hsvImage);
    cv::imshow("mask Image", mask);
    cv::imshow("mask1 Image", mask1);
    cv::imshow("mask2 Image", mask2);

    cv::Mat3b tewp;
    cv::Mat3b ss;
    cv::cvtColor(mask, tewp, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(inputImage, tewp, ss);
    cv::imshow("mask2 Image", ss);
    cv::waitKey(0);
    return 0;
}
