#include "../include/ImageProcessing.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

int main(int argc, char** argv)
{
    int lastSpeedLimit = 0;

    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("debug_file", "/dev/null");
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    std::string videoFile = "video/znak2.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return -1;
    }

    cv::namedWindow("Preview", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame)) { // read next frame
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        cv::waitKey(20); // change if calculation is too fast/slow
        updateImageView(frame, ocr, lastSpeedLimit);
    }

    ocr->End();
    delete ocr;
}
