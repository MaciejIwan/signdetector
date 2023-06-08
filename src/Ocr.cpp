#include "../include/Ocr.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include "../include/Common.h"
#include "../include/ImageProcessing.h"

Ocr::Ocr() : ocr(new tesseract::TessBaseAPI()) {
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    preprocessVector.push_back([this](cv::Mat roi) { return ImageProcessing::rawPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return ImageProcessing::adaptiveBrightnessPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return ImageProcessing::binaryDarkPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return ImageProcessing::binaryBrightPreprocess(roi); });
}

Ocr::~Ocr() {
    std::cout << "Ocr destructor" << std::endl;
    delete ocr;
    ocr = nullptr;
}

std::vector<std::function<cv::Mat(cv::Mat)>> &Ocr::getpreprocessVector() {
    return preprocessVector;
}

int Ocr::getNumberFromRoi(cv::Mat &roi, const std::function<cv::Mat(cv::Mat)> &preprocessFunction) {
    int value = 0;  // default value

    cv::Mat preprocessedRoi = preprocessFunction(roi);
    ocr->SetImage(preprocessedRoi.data, preprocessedRoi.cols, preprocessedRoi.rows, preprocessedRoi.channels(),
                  preprocessedRoi.step);

    char *text = ocr->GetUTF8Text();
    std::string stringText(text);
    delete[] text;

    if (DEBUG_MODE) {
        if (DEBUG_OCR_CONSOLE_LOG)
            std::cout << "OCR: " << stringText << std::endl;

        if (DEBUG_OCR_IMG) {
            cv::imwrite("output.jpg", roi);
            cv::imshow("preprocessedRoi ROI", preprocessedRoi);
            cv::waitKey(DEBUG_OCR_IMG_DELAY);
        }
    }

    try {
        value = filtrOcrOutput(stringText);
    } catch (...) {
        return 0;
    }
    return value;
}

int Ocr::filtrOcrOutput(std::string &input) {
    std::string text = replaceParenthesesWithWhitespace(input);
    return std::stoi(trim(text));
}

std::string Ocr::ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string Ocr::rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string Ocr::trim(const std::string &s) {
    std::string trimmedString = rtrim(ltrim(s));

    // Remove whitespace between characters
    size_t pos = trimmedString.find_first_of(WHITESPACE);
    while (pos != std::string::npos) {
        size_t next = trimmedString.find_first_not_of(WHITESPACE, pos);
        trimmedString.erase(pos, next - pos);
        pos = trimmedString.find_first_of(WHITESPACE, pos);
    }

    return trimmedString;
}

std::string Ocr::replaceParenthesesWithWhitespace(const std::string &s) {
    std::string replacedString = s;

    std::replace(replacedString.begin(), replacedString.end(), '(', ' ');
    std::replace(replacedString.begin(), replacedString.end(), ')', ' ');

    return replacedString;
}
