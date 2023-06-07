#include <string>
#include <opencv2/opencv.hpp>
#include "../include/Common.h"
#include "../include/CircularRoadSignDetector.h"
#include "../include/App.h"


App::App(int &argc, char **argv, FrameProvider* frameProvider, IRoadSignDetector* detector) : QApplication(argc, argv) {
    this->frameProvider = frameProvider;
    this->detector = detector;
    isDarkModeOn = true;
    isMuted = false;
    isClosed = false;
    relativePath = "sound/notification_sound.mp3";
    notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);
    frameLabel = new QLabel(&window);
    layout = new QVBoxLayout(&window);
    paintedSignDrawer = new SignDrawer(&window);
    muteButton = new QPushButton("Mute", &window);
    themeButton = new QPushButton("Light Mode", &window);
    init();
}

App * App::init() {
    window.setFixedSize(400, 600);

    frameLabel->setGeometry(0, 0, 600, 600);

    layout->addWidget(paintedSignDrawer);

    muteButton->setGeometry(30, 500, 150, 80);

    QObject::connect(muteButton, &QPushButton::clicked, [this]() {
        App::changeVolume();
    });

    themeButton->setGeometry(220, 500, 150, 80);
    QObject::connect(themeButton, &QPushButton::clicked, [this]() {
        App::changeMode();
    });

    themeButton->setStyleSheet("background-color: gray; color: white;");
    muteButton->setStyleSheet("background-color: gray; color: white;");

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, 400, 600);
    window.show();

    QObject::connect(&window, &QWidget::destroyed, [this](){
        isClosed=true;
    });

    detector->setNotificationCallback([&]() {
        notificationPlayer.play();
    });

    return this;
}

void App::changeMode() {
    isDarkModeOn = !isDarkModeOn;
    if(isDarkModeOn){
        themeButton->setText("Light Mode");
        themeButton->setStyleSheet("background-color: gray; color: white;");
        muteButton->setStyleSheet("background-color: gray; color: white;");
    }
    else{
       themeButton->setText("Dark Mode");
       themeButton->setStyleSheet("background-color: white; color: black;");
       muteButton->setStyleSheet("background-color: white; color: black;");
    }
    paintedSignDrawer->setThemeMode(isDarkModeOn);
}

void App::changeVolume() {
    isMuted = !isMuted;
    notificationPlayer.changeVolume(isMuted);
    isMuted ? muteButton->setText("Unmute") : muteButton->setText("Mute");
}


float calcFPS(int64* prevTickCount)
{
    int64 curTickCount = cv::getTickCount();
    double timeElapsed = (double)(curTickCount - *prevTickCount) / cv::getTickFrequency();
    *prevTickCount = curTickCount;
    return 1 / timeElapsed;
}

cv::Mat App::gui_filter_image(cv::Mat& raw, bool darkmode)
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

int App::exec() {
    cv::Mat frameImg, filtered;

    while (!isClosed) {

        frameImg = frameProvider->getFrame();
        if(DEBUG_MODE)
            cv::imshow("Raw", frameImg);

        auto* sign = (SpeedLimitSign*)detector->detectRoadSign(frameImg);

        filtered = gui_filter_image(frameImg, isDarkModeOn); // false for light mode

        QPixmap pixmap = QPixmap::fromImage(
                QImage((unsigned char*)filtered.data, filtered.cols, filtered.rows, QImage::Format_RGB888)); //QImage::Format_BGR888
        frameLabel->setPixmap(pixmap);
        paintedSignDrawer->setSpeedText(QString::fromStdString(std::to_string(sign->getLimit())));

        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();
    }
    return 0;
}

App::~App() {
    delete frameLabel;
    delete muteButton;
    delete themeButton;
    delete layout;
    delete paintedSignDrawer;

}
