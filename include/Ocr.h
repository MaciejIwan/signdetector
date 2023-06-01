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

    int filtrOcrOutput(std::string input);
    std::string trim(const std::string &s);

    cv::Mat preprocess1(cv::Mat roi);
    cv::Mat preprocess2(cv::Mat roi);
    cv::Mat preprocess3(cv::Mat roi);
    cv::Mat preprocess4(cv::Mat roi);
    cv::Mat preprocess5(cv::Mat roi);
    cv::Mat preprocess6(cv::Mat roi, int thresholdValue);

    using preprocessFun = std::function<cv::Mat(cv::Mat)>;
    std::vector<preprocessFun> preprocessVector;
};
