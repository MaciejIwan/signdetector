#include "../include/Ocr.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <regex>
#include <string>

#include "../include/Common.h"

Ocr::Ocr() : ocr(new tesseract::TessBaseAPI()) {
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    preprocessVector.push_back([this](cv::Mat roi) { return rawPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return adaptiveBrightnessPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return binaryDarkPreprocess(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return binaryBrightPreprocess(roi); });

}

Ocr::~Ocr() {
    ocr->End();
    delete ocr;
}

cv::Mat Ocr::adaptiveBrightnessPreprocess(cv::Mat roi) {
    double alpha = 1.5;  // contrast control, adjust as needed
    int beta = 20;       // brightness control, adjust as needed
    cv::Mat result;
    roi.convertTo(result, -1, alpha, beta);

    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(result, result);
    cv::bitwise_not(result, result);

    cv::adaptiveThreshold(result, result, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 81, 0);

    cv::erode(result, result, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::dilate(result, result, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    return result;
}

cv::Mat Ocr::binaryDarkPreprocess(cv::Mat roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::threshold(roi, roi, 100, 255, cv::THRESH_BINARY);

    return roi;
}

cv::Mat Ocr::binaryBrightPreprocess(cv::Mat roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::threshold(roi, roi, 200, 255, cv::THRESH_BINARY);

    return roi;
}

cv::Mat Ocr::rawPreprocess(cv::Mat roi) {
    return roi;
}

int Ocr::getNumberFromRoi(cv::Mat &roi) {
    int value = 0;  // default value

    for (const auto &preprocessFunc : preprocessVector) {
        cv::Mat preprocessedRoi = preprocessFunc(roi);
        ocr->SetImage(preprocessedRoi.data, preprocessedRoi.cols, preprocessedRoi.rows, preprocessedRoi.channels(), preprocessedRoi.step);

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

            if (value % 10 != 0)  // todo maybe use sign whitelist here?
                continue;
            break;
        } catch (...) {
            continue;
        }
    }

    return value;
}

int Ocr::filtrOcrOutput(std::string input) {
    std::string text = trim(input);

    std::regex re("[^0-9]?\\d+[^0-9]?");
    std::smatch match;

    std::regex_match(text, match, re);
    std::regex_search(text, match, std::regex("\\b\\d+\\b"));

    return std::stoi(match.str());
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
