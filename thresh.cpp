#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int thresholdValue = 128;
int colorChannel = 0;
int currentImageIndex = 0;

Mat originalImage, thresholdedImage;
vector<cv::String> fn;
vector<Mat> images;

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

void onImageTrackbar(int, void*)
{
    currentImageIndex = getTrackbarPos("Image", "Thresholded Image");
    originalImage = images[currentImageIndex];

    updateThresholdedImage();
}

int main()
{

    glob("imgs/*.jpg", fn, false);
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++)
        images.push_back(imread(fn[i]));

    if (images.empty())
    {
        cout << "Could not read the image: " << endl;
        return 1;
    }

    originalImage = images[currentImageIndex];

    // Create a window to display the original and thresholded images
    namedWindow("Thresholded Image", WINDOW_NORMAL);

    // Create a threshold trackbar
    createTrackbar("Threshold Value", "Thresholded Image", &thresholdValue, 255, onThresholdTrackbar);

    createTrackbar("Image", "Thresholded Image", &currentImageIndex, images.size()-1, onImageTrackbar);

    // Create a color channel dropdown
    createTrackbar("Color Channel", "Thresholded Image", &colorChannel, 3, onThresholdTrackbar);
    setTrackbarMin("Color Channel", "Thresholded Image", 0);
    setTrackbarMax("Color Channel", "Thresholded Image", 3);

    // Initialize the thresholded image with the default threshold value and grayscale option
    updateThresholdedImage();

    // Wait for the user to press a key
    waitKey(0);

    return 0;
}
