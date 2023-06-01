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

    preprocessVector.push_back([this](cv::Mat roi) { return preprocess1(roi); });
    preprocessVector.push_back([this](cv::Mat roi) { return preprocess2(roi); });
}

Ocr::~Ocr() {
    ocr->End();
    delete ocr;
}

cv::Mat Ocr::preprocess1(cv::Mat roi) {
    double alpha = 1.5;  // contrast control, adjust as needed
    int beta = 20;       // brightness control, adjust as needed
    cv::Mat result;
    roi.convertTo(result, -1, alpha, beta);

    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(result, result);
    cv::bitwise_not(result, result);

    cv::adaptiveThreshold(result, result, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 81, 0);
    // cv::threshold(roi, roi, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    // cv::threshold(roi, roi, 90, 255, cv::THRESH_BINARY_INV);

    cv::erode(result, result, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::dilate(result, result, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    return roi;
}

cv::Mat Ocr::preprocess2(cv::Mat roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(roi, roi);
    
    cv::threshold(roi, roi, 90, 255, cv::THRESH_BINARY);

    cv::erode(roi, roi, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::dilate(roi, roi, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    return roi;
}

int Ocr::getNumberFromRoi(cv::Mat &roi) {
    int value = 0;  // default value

    for (const auto& preprocessFunc : preprocessVector) {
        cv::Mat preprocessedRoi = preprocessFunc(roi);

        ocr->SetImage(preprocessedRoi.data, preprocessedRoi.cols, preprocessedRoi.rows, 1, preprocessedRoi.step);

        if (DEBUG_MODE) {
            if (DEBUG_OCR_CONSOLE_LOG)
                std::cout << "OCR: " << ocr->GetUTF8Text() << std::endl;

            if (DEBUG_OCR_IMG) {
                cv::imwrite("output.jpg", roi);
                cv::imshow("preprocessedRoi ROI", preprocessedRoi);
                cv::waitKey(DEBUG_OCR_IMG_DELAY);
            }
        }        

        try {
            value = filtrOcrOutput(ocr->GetUTF8Text());
            if (value % 10 != 0) //todo maybe use sign whitelist here?
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

    if (DEBUG_OCR_CONSOLE_LOG)
        std::cout << "match: " << match.str() << std::endl;

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
    return rtrim(ltrim(s));
}
