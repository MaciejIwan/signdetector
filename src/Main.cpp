#include "../include/CircularBuffer.h"
#include "../include/CircularRoadSignDetector.h"
#include "../include/Common.h"
#include "../include/FrameProvider.h"
#include "../include/IRoadSignDetector.h"
#include "../include/App.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"


#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <string>

float calcFPS(int64* prevTickCount);

cv::Mat gui_filter_image(cv::Mat& raw, bool darkmode)
{
    // Applies simple Sobel edge detection to simplify
    // the raw image and make it more readable for GUI

    int ksize = 3;
    int ksize_blur = 5;
    int delta = 1;
    int ddepth = CV_16S;

    cv::Mat output, grayscale;

    cv::GaussianBlur(raw, output, cv::Size(ksize_blur, ksize_blur), 0);
    cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y, grad;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(output, grad_x, ddepth, 1, 0, ksize, 1, delta, cv::BORDER_DEFAULT);
    cv::Sobel(output, grad_y, ddepth, 0, 1, ksize, 1, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    if (!darkmode) {
        cv::bitwise_not(grad, grad);
    }

    cv::cvtColor(grad, output, cv::COLOR_GRAY2RGB);

    return output;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    App uiApp;
    uiApp.init();

    IRoadSignDetector* detector = new CircularRoadSignDetector();

    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    FrameProvider frameProvider = FrameProvider(videoFile); // is it checking if file exists?

    detector->setNotificationCallback([&uiApp]() {
        uiApp.notificationPlayer.play();
    });
    cv::Mat frame, filtered;
    int64 prevTickCount = cv::getTickCount();

    while (true) {

        frame = frameProvider.getFrame();
        auto* sign = (SpeedLimitSign*)detector->detectRoadSign(frame);

        filtered = gui_filter_image(frame, uiApp.isDarkModeOn); // false for light mode

        QPixmap pixmap = QPixmap::fromImage(
            QImage((unsigned char*)filtered.data, filtered.cols, filtered.rows, QImage::Format_BGR888));
        uiApp.frame->setPixmap(pixmap);
        uiApp.fpsLabel->setText("fps: " + QString::number(calcFPS(&prevTickCount)));
        uiApp.paintedSign->setSpeedText(QString::fromStdString(std::to_string(sign->getLimit())));

        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();
    }

    return app.exec();
}


float calcFPS(int64* prevTickCount)
{
    int64 curTickCount = cv::getTickCount();
    double timeElapsed = (double)(curTickCount - *prevTickCount) / cv::getTickFrequency();
    *prevTickCount = curTickCount;
    return 1 / timeElapsed;
}
