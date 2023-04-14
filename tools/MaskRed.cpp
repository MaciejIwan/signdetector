#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;

int currentImageIndex = 0;
cv::Mat currImage, thresholdedImage;

vector<cv::String> fn;
vector<cv::Mat> images;
vector<cv::Mat> hsv_images;

// Define red color range
cv::Scalar lower_red1(0, 50, 30);
cv::Scalar upper_red1(10, 255, 255);
cv::Scalar lower_red2(160, 50, 30);
cv::Scalar upper_red2(180, 255, 255);
cv::Scalar lower_red_pink(140, 50, 50);
cv::Scalar upper_red_pink(160, 255, 255);
cv::Scalar lower_red_claret(0, 50, 10);
cv::Scalar upper_red_claret(10, 255, 150);
cv::Scalar lower_red_dark(0, 5, 0);
cv::Scalar upper_red_dark(10, 90, 50);


void updateImageView();



double compareContoursToCircle(const std::vector<cv::Point>& contour) {
    // Fit a minimum enclosing circle to the contour
    cv::Point2f center;
    float radius;
    cv::minEnclosingCircle(contour, center, radius);

    // Calculate the area of the contour and the area of the enclosing circle
    double contour_area = cv::contourArea(contour);
    double circle_area = CV_PI * radius * radius;

    // Calculate the similarity ratio between the contour and the circle
    double similarity = contour_area / circle_area;

    return similarity;
}

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

    cv::Mat masked_image;
    currImage.copyTo(masked_image, red_mask);

    // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    // Create a copy of the original image for contour preview
    cv::Mat contour_preview = currImage.clone();

    // Iterate through each contour and check if it is a circle
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 2048) {
            cv::Rect bounding_rect = cv::boundingRect(contour);
            double contour_similarity = compareContoursToCircle(contour);
            if (contour_similarity >= 0.6) {
                // Draw a green bounding box on the original image
                cv::rectangle(currImage, bounding_rect, cv::Scalar(0, 255, 0), 2);
            }
            // Draw the contour on the contour preview image
            cv::drawContours(contour_preview, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 255, 0), 2);
            std::stringstream similarity_text;
            similarity_text << "Similarity: " << std::fixed << std::setprecision(2) << contour_similarity;
            cv::putText(contour_preview, similarity_text.str(), cv::Point(bounding_rect.x, bounding_rect.y + bounding_rect.height + 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
    }

    // Display original and masked images, as well as contour preview
    cv::imshow("Original Image", currImage);
    cv::imshow("Masked Image", masked_image);
    cv::imshow("Contour Preview", contour_preview);


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
