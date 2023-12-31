#pragma once

#include <tesseract/baseapi.h>

#include <opencv2/core/mat.hpp>

class Ocr {
   public:
    Ocr();

    ~Ocr();

    int getNumberFromRoi(cv::Mat &roi, const std::function<cv::Mat(cv::Mat)> &preprocessFunction);
    std::vector<std::function<cv::Mat(cv::Mat)>> &getpreprocessVector();

   private:
    const std::string WHITESPACE = " \n\r\t\f\v";
    tesseract::TessBaseAPI *ocr;

    std::string replaceParenthesesWithWhitespace(const std::string &s);
    std::string rtrim(const std::string &s);
    std::string ltrim(const std::string &s);
    std::string trim(const std::string &s);

    int filtrOcrOutput(std::string &input);



    std::vector<std::function<cv::Mat(cv::Mat)>> preprocessVector;
};
