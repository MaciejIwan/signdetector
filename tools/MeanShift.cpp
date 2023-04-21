#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    // Load the input image
    Mat inputImage = imread("imgs/0.jpg");

    // Convert the image to the Lab color space
    Mat labImage;
    cvtColor(inputImage, labImage, COLOR_BGR2Lab);

    // Set the parameters for the mean-shift algorithm
    TermCriteria criteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);
    int spatialRadius = 10;
    int colorRadius = 20;

    // Apply the mean-shift algorithm to the image
    Mat outputImage;
    pyrMeanShiftFiltering(labImage, outputImage, spatialRadius, colorRadius, 0, criteria);

    // Display the input and output images
    namedWindow("Input", WINDOW_NORMAL);
    namedWindow("Output", WINDOW_NORMAL);
    imshow("Input", inputImage);
    imshow("Output", outputImage);
    waitKey(0);

    // Clean up
    destroyAllWindows();
    return 0;
}