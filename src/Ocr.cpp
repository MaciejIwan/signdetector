#include "../include/Ocr.h"
#include <regex>
#include "../include/Common.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

Ocr::Ocr() : ocr(new tesseract::TessBaseAPI()) {
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

Ocr::~Ocr() {
    ocr->End();
    delete ocr;
}

cv::Mat Ocr::preprocess(cv::Mat roi) {
    double alpha = 1.5; // contrast control, adjust as needed
    int beta = 20; // brightness control, adjust as needed
    cv::Mat result;
    roi.convertTo(result, -1, alpha, beta);

    if (DEBUG_OCR_IMG && DEBUG_MODE) {
        cv::imshow("contrast ROI", result);
        cv::imshow("Orignal roi", roi);
    }
    roi = result;

    cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(roi, roi);
    cv::bitwise_not(roi, roi);

    cv::adaptiveThreshold(roi, roi, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 81, 0);
    //cv::threshold(roi, roi, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    //cv::threshold(roi, roi, 90, 255, cv::THRESH_BINARY_INV);

    cv::erode(roi, roi, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::dilate(roi, roi, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    return roi;
}

int Ocr::getNumberFromRoi(cv::Mat &roi) {
    cv::Mat preprocessedRoi = preprocess(roi);
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

    std::string text = trim(std::string(ocr->GetUTF8Text()));

    std::regex re("[^0-9]?\\d+[^0-9]?");
    std::smatch match;
    std::regex_match(text, match, re);
    try {
        std::regex_search(text, match, std::regex("\\b\\d+\\b"));
        std::cout << "match: " << match.str() << std::endl;
        return std::stoi(match.str());
    } catch (...) {
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


