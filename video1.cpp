#include <opencv2/opencv.hpp>


cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);

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

void updateImageView(cv::Mat &currentFrame) {

    cv::Mat hsvFrame;
    cv::cvtColor(currentFrame, hsvFrame, cv::COLOR_BGR2HSV);

    cv::Mat red_mask = extractRedColorFromImage(hsvFrame);

    cv::Mat maskedFrame;
    currentFrame.copyTo(maskedFrame, red_mask);

    // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat contourPreview = currentFrame.clone();

    // Iterate through each contour and check if it is a circle
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 750) {
            cv::Rect bounding_rect = cv::boundingRect(contour);
            double contour_similarity = compareContoursToCircle(contour);
            if (contour_similarity >= 0.60) {
                cv::rectangle(currentFrame, bounding_rect, cv::Scalar(0, 255, 0), 2);
            }

            cv::drawContours(contourPreview, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 255, 0), 2);
            std::stringstream similarity_text;
            similarity_text << "Similarity: " << std::fixed << std::setprecision(2) << contour_similarity;
            cv::putText(contourPreview, similarity_text.str(), cv::Point(bounding_rect.x, bounding_rect.y + bounding_rect.height + 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
    }

    // Display original and masked images, as well as contour preview
    cv::imshow("Preview", currentFrame);
    cv::imshow("Masked Image", red_mask);
    cv::imshow("Contour Preview", contourPreview);

}

cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame) {
    static cv::Scalar lower_red1(0, 50, 30);
    static cv::Scalar upper_red1(10, 255, 255);
    static cv::Scalar lower_red2(160, 50, 30);
    static cv::Scalar upper_red2(180, 255, 255);
    static cv::Scalar lower_red_pink(140, 50, 50);
    static cv::Scalar upper_red_pink(160, 255, 255);
    static cv::Scalar lower_red_claret(0, 50, 10);
    static cv::Scalar upper_red_claret(10, 255, 150);
    static cv::Scalar lower_red_dark(0, 5, 0);
    static cv::Scalar upper_red_dark(10, 90, 50);

    cv::Mat red_mask, red_mask1, red_mask2, red_mask_pink, red_mask_claret, red_mask_dark;
    cv::inRange(hsvFrame, lower_red1, upper_red1, red_mask1);
    cv::inRange(hsvFrame, lower_red2, upper_red2, red_mask2);
    cv::inRange(hsvFrame, lower_red_pink, upper_red_pink, red_mask_pink);
    cv::inRange(hsvFrame, lower_red_claret, upper_red_claret, red_mask_claret);
    cv::inRange(hsvFrame, lower_red_dark, upper_red_dark, red_mask_dark);
    cv::bitwise_or(red_mask1, red_mask2, red_mask);
    cv::bitwise_or(red_mask, red_mask_pink, red_mask);
    cv::bitwise_or(red_mask, red_mask_claret, red_mask);
    cv::bitwise_or(red_mask, red_mask_dark, red_mask);

    return red_mask;
}

int main(int argc, char** argv)
{
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    cv::VideoCapture cap("video/znak2.mp4");

    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file " << std::endl;
        return -1;
    }

    cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true)
    {
        if (!cap.read(frame)) // read next frame
        {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        cv::waitKey(20); // change if calculation is too fast/slow
        updateImageView(frame);
    }
}
