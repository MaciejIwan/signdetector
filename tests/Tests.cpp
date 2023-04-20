
#include "include/utils/VideoTestUtils.h"
#include <gtest/gtest.h>
#include <string>

TEST(VideoTest_speed_limit_seqence_1, SignRecognitionAccuracy)
{
    std::string filename = "speed_limit_seqence_1";
    testSignRecognitionAccuracy(filename);
}

TEST(VideoTest_speed_limit_11, SignRecognitionAccuracy)
{
    std::string filename = "speed_limit_11";
    testSignRecognitionAccuracy(filename);
}
