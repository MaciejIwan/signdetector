#pragma once

#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

class Ocr {
public:
    Ocr();

    ~Ocr();

    int getNumberFromRoi(cv::Mat &roi);

private:
    const std::string WHITESPACE = " \n\r\t\f\v";
    tesseract::TessBaseAPI *ocr;

    std::string rtrim(const std::string &s);

    std::string ltrim(const std::string &s);

    std::string trim(const std::string &s);

    cv::Mat preprocess(cv::Mat roi);
};
