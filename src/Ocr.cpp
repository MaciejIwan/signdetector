//
// Created by maciej on 14.04.23.
//

#include <string>
#include <regex>
#include "../include/common.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "../include/Ocr.h"

Ocr::Ocr() : ocr(new tesseract::TessBaseAPI()) {
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_AUTO);
}

Ocr::~Ocr() {
    ocr->End();
    delete ocr;
}

int Ocr::getNumberFromRoi(const cv::Mat &roi) {
    ocr->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);
    if(DEBUG_MODE){
        cv::imshow("ROI", roi);
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


