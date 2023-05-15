#include <gtest/gtest.h>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "../include/Ocr.h"

class RoiOcrTest : public ::testing::Test {
public:
    static void testTextReading(const std::string &filename) {
        //given
        Ocr ocr = Ocr();
        int expectedSpeedLimit = std::stoi(filename);

        //when
        cv::Mat image = cv::imread("../resources/imgs/signs/" + filename);
        int actualSpeedLimit = ocr.getNumberFromRoi(image);

        // then
        ASSERT_EQ(expectedSpeedLimit, actualSpeedLimit);
    }
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};




TEST_F(RoiOcrTest, shouldReturn30_1) {
    std::string filename = "30_1.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn30_2) {
    std::string filename = "30_2.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn30_3) {
    std::string filename = "30_3.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_1) {
    std::string filename = "40_1.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_4) {
    std::string filename = "40_4.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_5) {
    std::string filename = "40_5.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn50_1) {
    std::string filename = "50_1.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn60_1) {
    std::string filename = "60_1.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn60_2) {
    std::string filename = "60_2.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn70_1_blackBackground) {
    std::string filename = "70_1.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn70_2_strongReflation) {
    std::string filename = "70_2.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn70_3) {
    std::string filename = "70_3.jpg";
    testTextReading(filename);
}