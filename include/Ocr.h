#pragma once

#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

class Ocr {
public:
    Ocr();

    ~Ocr();

    int getNumberFromRoi(cv::Mat &roi, std::function<cv::Mat(cv::Mat)> preprocessFunction);
    std::vector<std::function<cv::Mat(cv::Mat)>>& getpreprocessVector();

private:
    const std::string WHITESPACE = " \n\r\t\f\v";
    tesseract::TessBaseAPI *ocr;

    std::string rtrim(const std::string &s);
    std::string ltrim(const std::string &s);
    std::string trim(const std::string &s);

    int filtrOcrOutput(std::string input);

    cv::Mat adaptiveBrightnessPreprocess(cv::Mat roi);
    cv::Mat binaryDarkPreprocess(cv::Mat roi);
    cv::Mat binaryBrightPreprocess(cv::Mat roi);
    cv::Mat rawPreprocess(cv::Mat roi);

    std::vector<std::function<cv::Mat(cv::Mat)>> preprocessVector;
};
