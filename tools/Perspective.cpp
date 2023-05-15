#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    // Load image
    cv::Mat src = cv:: imread("imgs/0.jpg");

    if (src.empty()) {
        std::cerr << "Failed to load image" << std::endl;
        return -1;
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // Apply threshold
    cv::Mat thresh;
    cv::threshold(gray, thresh, 120, 255, cv::THRESH_BINARY);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Find largest contour
    int largest_contour_idx = 0;
    double largest_area = 0;
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > largest_area) {
            largest_area = area;
            largest_contour_idx = i;
        }
    }

    // Fit ellipse to largest contour
    cv::RotatedRect ellipse = cv::fitEllipse(contours[largest_contour_idx]);

    // Create mask
    cv::Mat mask(src.size(), CV_8UC1, cv::Scalar(0));
    cv::ellipse(mask, ellipse, cv::Scalar(255), -1);
    cv::imshow("mask", mask);
    // Perspective transform
    cv::Mat warped;
    std::vector<cv::Point2f> src_pts = { ellipse.center + cv::Point2f(-ellipse.size.width / 2, -ellipse.size.height / 2),
                                         ellipse.center + cv::Point2f(-ellipse.size.width / 2, ellipse.size.height / 2),
                                         ellipse.center + cv::Point2f(ellipse.size.width / 2, ellipse.size.height / 2),
                                         ellipse.center + cv::Point2f(ellipse.size.width / 2, -ellipse.size.height / 2) };
    std::vector<cv::Point2f> dst_pts = { cv::Point2f(0, 0), cv::Point2f(0, src.rows), cv::Point2f(src.cols, src.rows), cv::Point2f(src.cols, 0) };
    cv::Mat transform = cv::getPerspectiveTransform(src_pts, dst_pts);
    cv::warpPerspective(src, warped, transform, src.size());

    // Show images
    cv::imshow("Original", src);
    cv::imshow("Warped", warped);
    cv::waitKey(0);

    return 0;
}
