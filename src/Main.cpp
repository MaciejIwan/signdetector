#include <regex>

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

const double MIN_CONTOUR_SIMILARITY = 0.75;
const std::string WHITESPACE = " \n\r\t\f\v";

cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);

std::string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

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

void blurImage(cv::Mat &image, int size, double sigma) {
    cv::Size kernelSize = cv::Size(size, size);
    //cv::GaussianBlur(image, image, kernelSize, sigma);
    cv::medianBlur(image, image, size);
}

int getNumberFromRoi(const cv::Mat& roi, tesseract::TessBaseAPI *ocr) {
    ocr->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);
    std::string text = trim(std::string(ocr->GetUTF8Text()));

    std::regex re("\\(?\\d+\\)?");
    std::smatch match;
    bool matchFound = std::regex_match(text, match, re);
    if (matchFound) {
        std::regex_search(text, match, std::regex("\\b\\d+\\b"));
        return std::stoi(match.str());
    } else {
        return 0;
    }
}


void updateImageView(cv::Mat &currentFrame, tesseract::TessBaseAPI *ocr, int& lastSpeedLimit) {
    blurImage(currentFrame, 5, 0);

    cv::Mat hsvFrame;
    cv::cvtColor(currentFrame, hsvFrame, cv::COLOR_BGR2HSV);



    cv::Mat red_binary_mask = extractRedColorFromImage(hsvFrame);

    // Zdefiniuj rozmiar jądra dla operacji morfologicznych
    cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    // Zastosuj operację otwarcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_OPEN, kernel);
    // Zastosuj operację zamknięcia
    morphologyEx(red_binary_mask, red_binary_mask, cv::MORPH_CLOSE, kernel);

    cv::Mat maskedFrame;
    currentFrame.copyTo(maskedFrame, red_binary_mask);

    // Find contours in the masked image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_binary_mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat contourPreview = currentFrame.clone();
    cv::Mat temp = currentFrame.clone(); // frame for tesseract

    // Iterate through each contour and check if it is a circle
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 750) {
            cv::Rect bounding_rect = cv::boundingRect(contour);
            double contour_similarity = compareContoursToCircle(contour);
            if (contour_similarity >= MIN_CONTOUR_SIMILARITY) {
                cv::Mat roi = temp(bounding_rect); // Extract the ROI from the current frame
                int numberFromRoi = getNumberFromRoi(roi, ocr);
                if (numberFromRoi != 0) {
                    lastSpeedLimit = numberFromRoi;
                }
                cv::rectangle(currentFrame, bounding_rect, cv::Scalar(0, 255, 0), 2);

            }
            cv::drawContours(contourPreview, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 255, 0), 2);
            std::stringstream similarity_text;
            similarity_text << "Similarity: " << std::fixed << std::setprecision(2) << contour_similarity;
            cv::putText(contourPreview, similarity_text.str(), cv::Point(bounding_rect.x, bounding_rect.y + bounding_rect.height + 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
    }
    std::stringstream sppedLimitText;
    sppedLimitText << "speed limit: " << lastSpeedLimit;
    cv::putText(currentFrame, //target image
            sppedLimitText.str(), //text
            cv::Point(10, currentFrame.rows / 5), //top-left position
            cv::FONT_HERSHEY_SIMPLEX,
            1.0,
            cv::Scalar(255, 0, 255), //font color
            2);

    // Display original and masked images, as well as contour preview
    cv::imshow("Preview", currentFrame);
    cv::imshow("Masked Image", maskedFrame);
    cv::imshow("red color mask", red_binary_mask);
    cv::imshow("Contour Preview", contourPreview);

}

cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame) {
    static cv::Scalar lower_red1(0, 50, 30);
    static cv::Scalar upper_red1(15, 255, 255);
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

int main(int argc, char** argv) {
    int lastSpeedLimit = 0;

    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    std::string videoFile = "video/znak2.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return -1;
    }

    cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame)) { // read next frame
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        cv::waitKey(20); // change if calculation is too fast/slow
        updateImageView(frame, ocr, lastSpeedLimit);
    }

    ocr->End();
    delete ocr;
}
