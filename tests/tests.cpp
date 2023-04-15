#include "../include/ImageProcessing.h"
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
    int lastSpeedLimit = 0;

    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    std::string videoFile = "video/znak2.mp4";

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return;
    }

    cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame)) { // read next frame
            return;
        }

        cv::waitKey(20); // change if calculation is too fast/slow
        updateImageView(frame, ocr, lastSpeedLimit);
    }

    ocr->End();
    delete ocr;

    // TODO: Placeholder test output - change to real test (question: what will qualify as test success?)

    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
