//
// Created by maciej on 20.04.23.
//

#include <gtest/gtest.h>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "../include/Ocr.h"
#include "../include/models/SpeedLimitSign.h"

TEST(OcrTest, shouldReturn40) {
    std::cout << "OcrTest: ImagePreprocessing" << std::endl;
    std::string filename = "imgs/signs/0.jpg";

    int expectedSign = 40;
    Ocr ocr = Ocr();

    cv::Mat image = cv::imread(filename);
    int actualSign = ocr.getNumberFromRoi(image);

    ASSERT_EQ(expectedSign, actualSign);
}

