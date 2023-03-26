#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int thresholdValue = 128;
int colorChannel = 0; // default grayscale
Mat originalImage, thresholdedImage;

void updateThresholdedImage()
{
    switch (colorChannel) {
        case 0: // grayscale option
            cvtColor(originalImage, thresholdedImage, COLOR_BGR2GRAY);
            break;
        case 1: // red channel option
        {
            Mat channels[3];
            split(originalImage, channels);
            channels[1] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            channels[2] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            merge(channels, 3, thresholdedImage);
        }
            break;
        case 2: // green channel option
        {
            Mat channels[3];
            split(originalImage, channels);
            channels[0] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            channels[2] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            merge(channels, 3, thresholdedImage);
        }
            break;
        case 3: // blue channel option
        {
            Mat channels[3];
            split(originalImage, channels);
            channels[0] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            channels[1] = Mat::zeros(originalImage.rows, originalImage.cols, CV_8UC1);
            merge(channels, 3, thresholdedImage);
        }
            break;
    }
    // apply thresholding
    threshold(thresholdedImage, thresholdedImage, thresholdValue, 255, THRESH_BINARY);
    imshow("Thresholded Image", thresholdedImage);
}

void onThresholdTrackbar(int, void*)
{
    updateThresholdedImage();
}

int main()
{
    // Load the original image
    originalImage = imread("imgs/img.jpg", IMREAD_COLOR);
    if (originalImage.empty())
    {
        cout << "Could not read the image: " << endl;
        return 1;
    }

    // Create a window to display the original and thresholded images
    namedWindow("Original Image", WINDOW_NORMAL);
    namedWindow("Thresholded Image", WINDOW_NORMAL);

    // Create a threshold trackbar
    createTrackbar("Threshold Value", "Thresholded Image", &thresholdValue, 255, onThresholdTrackbar);

    // Create a color channel dropdown
    createTrackbar("Color Channel", "Thresholded Image", &colorChannel, 3, onThresholdTrackbar);
    setTrackbarMin("Color Channel", "Thresholded Image", 0);
    setTrackbarMax("Color Channel", "Thresholded Image", 3);

    // Show the original image
    imshow("Original Image", originalImage);

    // Initialize the thresholded image with the default threshold value and grayscale option
    updateThresholdedImage();

    // Wait for the user to press a key
    waitKey(0);

    return 0;
}
