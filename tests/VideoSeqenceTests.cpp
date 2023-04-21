
#include "include/utils/VideoTestUtils.h"
#include <gtest/gtest.h>
#include <string>


class VideoSequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(VideoSequenceTest, VideoTest_speed_limit_seqence_1) {
    std::string filename = "speed_limit_seqence_1";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, VideoTest_speed_limit_11) {
    std::string filename = "speed_limit_0";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_1_video_with_lighting_black_sign) {
    std::string filename = "speed_limit_1";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_2_video_with_lighting_black_sign) {
    std::string filename = "speed_limit_2";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_3_huge_reflaction) {
    std::string filename = "speed_limit_3";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_4) {
    std::string filename = "speed_limit_4";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_5) {
    std::string filename = "speed_limit_5";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_6) {
    std::string filename = "speed_limit_6";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_7) {
    std::string filename = "speed_limit_7";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_8) {
    std::string filename = "speed_limit_8";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_9) {
    std::string filename = "speed_limit_9";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_10) {
    std::string filename = "speed_limit_10";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, speed_limit_11) {
    std::string filename = "speed_limit_11";
    testSignRecognitionAccuracy(filename);
}

