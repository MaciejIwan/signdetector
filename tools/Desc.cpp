#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void resizeImage(cv::Mat &img, int max_height) {
    double ratio = 1.0;
    if (img.rows > max_height) {
        ratio = float(max_height) / img.rows;
        cv::resize(img, img, cv::Size(), ratio, ratio);
    }
}


void mergeImages(const std::vector<cv::Mat> &images, cv::Mat &output) {
    // Check that there are exactly 4 input images
    CV_Assert(images.size() == 4);

    // Check that all input matrices have the same number of rows
    CV_Assert(images[0].rows == images[1].rows && images[0].rows == images[2].rows && images[0].rows == images[3].rows);

    // Convert grayscale photos to color
    std::vector<cv::Mat> color_images;
    for (const cv::Mat &image: images) {
        cv::Mat color_image;
        if (image.channels() == 1) {
            cv::cvtColor(image, color_image, cv::COLOR_GRAY2BGR);
        } else {
            color_image = image;
        }
        color_images.push_back(color_image);
    }

    cv::Mat top_row, bottom_row;
    cv::hconcat(color_images[0], color_images[1], top_row);
    cv::hconcat(color_images[2], color_images[3], bottom_row);
    cv::vconcat(top_row, bottom_row, output);
}

int main() {
    // Load the image
    Mat image = imread("imgs/img.jpg");

    // Convert the image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Apply thresholding to the image
    Mat thresholded;
    threshold(gray, thresholded, 150, 255, THRESH_BINARY);

    // Apply Canny edge detection
    Mat edges;
    Canny(thresholded, edges, 100, 200);

    // Find contours in the image
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Iterate through the contours and find the one that matches a street sign
    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > 500 && area < 5000) {
            Rect rect = boundingRect(contours[i]);
            double ratio = (double) rect.width / (double) rect.height;
            if (ratio > 0.8 && ratio < 1.2) {
                rectangle(image, rect, Scalar(0, 0, 255), 2);
            }
        }
    }


    // Display the grayscale thresholded image and edges in the preview window
    Mat preview;
//    mergeImages(image, gray, thresholded, edges, preview);
    mergeImages({image, gray, thresholded, edges}, preview);
    resizeImage(preview, 1280);
    imshow("Result", preview);
    waitKey(0);

    return 0;
}