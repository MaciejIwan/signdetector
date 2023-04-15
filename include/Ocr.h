//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_OCR_H
#define SIGN_EDGE_METHOD_TEST_1_OCR_H


#include <tesseract/baseapi.h>
#include <opencv2/core/mat.hpp>

class Ocr {
public:
    Ocr();

    ~Ocr();

    int getNumberFromRoi(const cv::Mat &roi);

private:
    const std::string WHITESPACE = " \n\r\t\f\v";
    tesseract::TessBaseAPI *ocr;

    std::string rtrim(const std::string &s);

    std::string ltrim(const std::string &s);

    std::string trim(const std::string &s);
};


#endif //SIGN_EDGE_METHOD_TEST_1_OCR_H
