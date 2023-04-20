//
// Created by maciej on 20.04.23.
//

#include <gtest/gtest.h>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "../include/Ocr.h"

void testTextReading(const std::string &filename) {
    int expectedSign = 40;
    Ocr ocr = Ocr();

    cv::Mat image = cv::imread("../test_images/" + filename);
    int actualSign = ocr.getNumberFromRoi(image);

    ASSERT_EQ(expectedSign, actualSign);
}

TEST(OcrTest, shouldReturn40)
{
    std::string filename = "40_1.png";
    testTextReading(filename);
}

TEST(OcrTest2, shouldReturn40)
{
    std::string filename = "40_2.png";
    testTextReading(filename);
}