//
// Created by maciej on 20.04.23.
//

#include <gtest/gtest.h>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "../include/Ocr.h"

void testTextReading(const std::string &filename) {
    //given
    Ocr ocr = Ocr();
    int expectedSpeedLimit = std::stoi(filename);

    //when
    cv::Mat image = cv::imread("../resources/imgs/signs/" + filename);
    int actualSpeedLimit = ocr.getNumberFromRoi(image);

    // then
    ASSERT_EQ(expectedSpeedLimit, actualSpeedLimit);
}

TEST(OcrTest, shouldReturn40) {
    std::string filename = "40_1.jpg";
    testTextReading(filename);
}

//TEST(OcrTest2, shouldReturn40)
//{
//    std::string filename = "40_2.jpg";
//    testTextReading(filename);
//}