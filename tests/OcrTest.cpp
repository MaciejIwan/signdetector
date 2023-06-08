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
        cv::Mat image = cv::imread("./img/signs/" + filename);
        int actualSpeedLimit = ocr.getNumberFromRoi(image, ocr.getpreprocessVector()[0]);

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

TEST_F(RoiOcrTest, shouldReturn30_4) {
    std::string filename = "30_4.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn30_5) {
    std::string filename = "30_5.jpg";
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

TEST_F(RoiOcrTest, shouldReturn40_6) {
    std::string filename = "40_6.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_7) {
    std::string filename = "40_7.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_8) {
    std::string filename = "40_8.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_9) {
    std::string filename = "40_9.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, shouldReturn40_10) {
    std::string filename = "40_10.jpg";
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

TEST_F(RoiOcrTest, shouldReturn60_3) {
    std::string filename = "60_3.jpg";
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

TEST_F(RoiOcrTest, shouldReturn70_black_4) {
    std::string filename = "70_black_4.jpg";
    testTextReading(filename);
}

TEST_F(RoiOcrTest, output) {
    Ocr ocr = Ocr();
    std::string filename = "40output.jpg";
    cv::Mat image = cv::imread("./img/signs/" + filename);

    int actualSpeedLimit = ocr.getNumberFromRoi(image, ocr.getpreprocessVector()[0]);

    ASSERT_EQ(40, actualSpeedLimit);
}