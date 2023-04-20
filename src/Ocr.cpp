//
// Created by maciej on 14.04.23.
//

#include <string>
#include <regex>
#include "../include/Common.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "../include/Ocr.h"

Ocr::Ocr() : ocr(new tesseract::TessBaseAPI()) {
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

Ocr::~Ocr() {
    ocr->End();
    delete ocr;
}

void Ocr::preprocess(cv::Mat &roi) {
    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    //cv::equalizeHist(roi, roi);

    cv::adaptiveThreshold(roi, roi, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 81, 80);
    cv::bitwise_not(roi, roi);


//    cv::dilate(roi, roi, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2)));
//    cv::erode(roi, roi, getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
}

int Ocr::getNumberFromRoi(cv::Mat &roi) {
    preprocess(roi);
    ocr->SetImage(roi.data, roi.cols, roi.rows, 1, roi.step);

    if (DEBUG_OCR && DEBUG_MODE) {
        cv::imshow("ROI", roi);
        cv::waitKey(DEBUG_OCR_IMG_DELAY);
        std::cout << "OCR: " << ocr->GetUTF8Text() << std::endl;
    }

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


