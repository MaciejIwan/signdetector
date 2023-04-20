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


TEST(shouldReturn30_1, OcrTest) {
    std::string filename = "30_1.jpg";
    testTextReading(filename);
}

TEST(shouldReturn30_2, OcrTest) {
    std::string filename = "30_2.jpg";
    testTextReading(filename);
}

TEST(shouldReturn30_3, shouldReturn30) {
    std::string filename = "30_3.jpg";
    testTextReading(filename);
}

TEST(shouldReturn40_1, shouldReturn40) {
    std::string filename = "40_1.jpg";
    testTextReading(filename);
}

TEST(shouldReturn40_2, shouldReturn40) {
    std::string filename = "40_2.jpg";
    testTextReading(filename);
}

TEST(shouldReturn40_3, shouldReturn40) {
    std::string filename = "40_3.jpg";
    testTextReading(filename);
}

TEST(shouldReturn40_4, shouldReturn40) {
    std::string filename = "40_4.jpg";
    testTextReading(filename);
}

TEST(shouldReturn40_5, shouldReturn40) {
    std::string filename = "40_5.jpg";
    testTextReading(filename);
}

TEST(shouldReturn50_1, shouldReturn50) {
    std::string filename = "50_1.jpg";
    testTextReading(filename);
}

TEST(shouldReturn60_1, shouldReturn60) {
    std::string filename = "60_1.jpg";
    testTextReading(filename);
}

TEST(shouldReturn60_2, shouldReturn60) {
    std::string filename = "60_2.jpg";
    testTextReading(filename);
}

TEST(shouldReturn70_1, shouldReturn70) {
    std::string filename = "70_1.jpg";
    testTextReading(filename);
}

TEST(shouldReturn70_2, shouldReturn70) {
    std::string filename = "70_2.jpg";
    testTextReading(filename);
}

TEST(shouldReturn70_3, shouldReturn70) {
    std::string filename = "70_3.jpg";
    testTextReading(filename);
}