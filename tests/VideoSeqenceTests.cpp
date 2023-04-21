
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

TEST_F(VideoSequenceTest, VideoTest_speed_limit_seqence_1)
{
    std::string filename = "speed_limit_seqence_1";
    testSignRecognitionAccuracy(filename);
}

TEST_F(VideoSequenceTest, VideoTest_speed_limit_11)
{
    std::string filename = "speed_limit_11";
    testSignRecognitionAccuracy(filename);
}
