#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;

int currentImageIndex = 0;
cv::Mat currImage, thresholdedImage;

vector<cv::String> fn;
vector<cv::Mat> images;
vector<cv::Mat> hsv_images;

// Define red color range
cv::Scalar lower_red(0, 30, 30);
cv::Scalar upper_red(10, 255, 255);
cv::Scalar lower_red2(170, 30, 30);
cv::Scalar upper_red2(180, 255, 255);


void updateImageView();

void onImageTrackbar(int, void*)
{
    currentImageIndex = cv::getTrackbarPos("Image", "Masked Image");
    currImage = images[currentImageIndex];

    updateImageView();
}

void updateImageView() {
    cv::Mat red_mask, red_mask2;
    cv::inRange(hsv_images[currentImageIndex], lower_red, upper_red, red_mask);
    cv::inRange(hsv_images[currentImageIndex], lower_red2, upper_red2, red_mask2);
    cv::bitwise_or(red_mask, red_mask2, red_mask);

    // Apply mask to image
    cv::Mat masked_image;
    currImage.copyTo(masked_image, red_mask);

    // Display original and masked images
    cv::imshow("Original Image", currImage);
    cv::imshow("Masked Image", masked_image);

}

int main() {
    // Load image
    cv::glob("imgs/*.jpg", fn, false);
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++){
        cv::Mat image;
        cv::resize(cv::imread(fn[i]), image, cv::Size(1280, 720));
        images.push_back(image);

    }

    if(images.empty()){
        std::cout << "Could not read the image: " << std::endl;
        return 1;
    }

    currImage = images[0];


    // Convert image to HSV color space

//    cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
    for_each(images.begin(), images.end(), [](cv::Mat &image) {
        cv::Mat hsv_image;
        cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
        hsv_images.push_back(hsv_image);
    });



//    // Create mask for red color
//    cv::Mat red_mask, red_mask2;
//    cv::inRange(hsv_images[0], lower_red, upper_red, red_mask);
//    cv::inRange(hsv_images[0], lower_red2, upper_red2, red_mask2);
//    cv::bitwise_or(red_mask, red_mask2, red_mask);
//
//    // Apply mask to image
//    cv::Mat masked_image;
//    images[0].copyTo(masked_image, red_mask);
//
//    // Display original and masked images
//    cv::imshow("Original Image", images[0]);
//    cv::imshow("Masked Image", masked_image);
    updateImageView();
    cv::createTrackbar("Image", "Masked Image", &currentImageIndex, images.size()-1, onImageTrackbar);
    cv::waitKey(0);

    return 0;
}
