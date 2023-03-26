#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;

int currentImageIndex = 0;
cv::Mat currImage, thresholdedImage;

vector<cv::String> fn;
vector<cv::Mat> images;
vector<cv::Mat> hsv_images;

// Define red color range
cv::Scalar lower_red1(0, 50, 50);
cv::Scalar upper_red1(10, 255, 255);
cv::Scalar lower_red2(170, 50, 50);
cv::Scalar upper_red2(180, 255, 255);
cv::Scalar lower_red_pink(140, 50, 50);
cv::Scalar upper_red_pink(170, 255, 255);
cv::Scalar lower_red_claret(0, 50, 20);
cv::Scalar upper_red_claret(10, 255, 150);
cv::Scalar lower_red_dark(0, 50, 0);
cv::Scalar upper_red_dark(10, 255, 50);


void updateImageView();

void onImageTrackbar(int, void *) {
    currentImageIndex = cv::getTrackbarPos("Image", "Masked Image");
    currImage = images[currentImageIndex];

    updateImageView();
}

void updateImageView() {

    // Create mask for red color
    cv::Mat red_mask, red_mask1, red_mask2, red_mask_pink, red_mask_claret, red_mask_dark;
    cv::inRange(hsv_images[currentImageIndex], lower_red1, upper_red1, red_mask1);
    cv::inRange(hsv_images[currentImageIndex], lower_red2, upper_red2, red_mask2);
    cv::inRange(hsv_images[currentImageIndex], lower_red_pink, upper_red_pink, red_mask_pink);
    cv::inRange(hsv_images[currentImageIndex], lower_red_claret, upper_red_claret, red_mask_claret);
    cv::inRange(hsv_images[currentImageIndex], lower_red_dark, upper_red_dark, red_mask_dark);
    cv::bitwise_or(red_mask1, red_mask2, red_mask);
    cv::bitwise_or(red_mask, red_mask_pink, red_mask);
    cv::bitwise_or(red_mask, red_mask_claret, red_mask);
    cv::bitwise_or(red_mask, red_mask_dark, red_mask);

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
    for (size_t i = 0; i < count; i++) {
        cv::Mat image;
        cv::resize(cv::imread(fn[i]), image, cv::Size(1280, 720));
        images.push_back(image);

    }

    if (images.empty()) {
        std::cout << "Could not read the image: " << std::endl;
        return 1;
    }

    currImage = images[0];

    for_each(images.begin(), images.end(), [](cv::Mat &image) {
        cv::Mat hsv_image;
        cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
        hsv_images.push_back(hsv_image);
    });


    updateImageView();
    cv::createTrackbar("Image", "Masked Image", &currentImageIndex, images.size() - 1, onImageTrackbar);
    cv::waitKey(0);

    return 0;
}
