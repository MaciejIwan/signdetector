//
// Created by maciej on 21.04.23.
//

#include "include/utils/VideoTestUtils.h"
#include <gtest/gtest.h>
#include <string>

class VideoTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(VideoTest, VideoTest_speed_limit_11) {
std::string filename = "speed_limit_0";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_1_video_with_lighting_black_sign) {
std::string filename = "speed_limit_1";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_2_video_with_lighting_black_sign) {
std::string filename = "speed_limit_2";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_3_huge_reflaction) {
std::string filename = "speed_limit_3";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_4) {
std::string filename = "speed_limit_4";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_5) {
std::string filename = "speed_limit_5";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_6) {
std::string filename = "speed_limit_6";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_7) {
std::string filename = "speed_limit_7";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_8) {
std::string filename = "speed_limit_8";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_9) {
std::string filename = "speed_limit_9";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_10) {
std::string filename = "speed_limit_10";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_11) {
std::string filename = "speed_limit_11";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_12) {
std::string filename = "speed_limit_night_12";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_13) {
std::string filename = "speed_limit_night_13";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_14) {
std::string filename = "speed_limit_night_14";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_15) {
std::string filename = "speed_limit_night_15";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_16) {
std::string filename = "speed_limit_night_16";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_17) {
std::string filename = "speed_limit_night_17";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_18) {
std::string filename = "speed_limit_night_18";
testSignRecognitionAccuracy(filename);
}

TEST_F(VideoTest, speed_limit_night_19) {
std::string filename = "speed_limit_night_19";
testSignRecognitionAccuracy(filename);
}