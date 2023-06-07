#include <string>
#include <opencv2/opencv.hpp>
#include "../include/Common.h"
#include "../include/CircularRoadSignDetector.h"
#include "../include/App.h"


App::App(int &argc, char **argv, FrameProvider *frameProvider, IRoadSignDetector *detector) : QApplication(argc, argv) {
    this->frameProvider = frameProvider;
    this->detector = detector;
    viewMode = ViewMode::NORMAL;
    isMuted = false;
    isClosed = false;
    relativePath = "sound/notification_sound.mp3";
    notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);
    framePreviewLabel = new QLabel(&window);
    paintedSignDrawer = new SignDrawer(150, &window) ;
    layout = new QVBoxLayout(&window);
    muteButton = new QPushButton("Mute", &window);
    themeButton = new QPushButton("Switch Mode", &window);
    init();
}

App *App::init() {
    int buttonWidth = 150;
    int buttonHeight = 80;
    int buttonSpace = 30;
    int buttonY = windowHeight - buttonHeight - buttonSpace;

    window.setFixedSize(windowWidth, windowHeight);

    framePreviewLabel->setGeometry(0, 0, windowWidth, windowHeight);

    layout->addWidget(paintedSignDrawer);

    muteButton->setGeometry((windowWidth / 2) - buttonWidth - buttonSpace, buttonY, buttonWidth, buttonHeight);

    QObject::connect(muteButton, &QPushButton::clicked, [this]() {
        App::changeVolume();
    });

    themeButton->setGeometry((windowWidth / 2) + buttonSpace, buttonY, buttonWidth, buttonHeight);
    QObject::connect(themeButton, &QPushButton::clicked, [this]() {
        App::changeMode();
    });

    themeButton->setStyleSheet("background-color: gray; color: white;");
    muteButton->setStyleSheet("background-color: gray; color: white;");

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, windowWidth, windowHeight);
    window.show();

    QObject::connect(&window, &QWidget::destroyed, [this]() {
        isClosed = true;
    });

    detector->setNotificationCallback([&]() {
        notificationPlayer.play();
    });

    return this;
}

void App::changeMode() {
    if (viewMode == DARK) {
        themeButton->setStyleSheet("background-color: gray; color: white;");
        muteButton->setStyleSheet("background-color: gray; color: white;");
        viewMode = ViewMode::LIGHT;
    } else if (viewMode == LIGHT) {
        themeButton->setStyleSheet("background-color: white; color: black;");
        muteButton->setStyleSheet("background-color: white; color: black;");
        viewMode = ViewMode::NORMAL;
    } else if (viewMode == NORMAL) {
        themeButton->setStyleSheet("background-color: white; color: black;");
        muteButton->setStyleSheet("background-color: white; color: black;");
        viewMode = ViewMode::DARK;
    }

    paintedSignDrawer->setThemeMode(viewMode == ViewMode::DARK);
}

void App::changeVolume() {
    isMuted = !isMuted;
    notificationPlayer.changeVolume(isMuted);
    isMuted ? muteButton->setText("Unmute") : muteButton->setText("Mute");
}


float calcFPS(int64 *prevTickCount) {
    int64 curTickCount = cv::getTickCount();
    double timeElapsed = (double) (curTickCount - *prevTickCount) / cv::getTickFrequency();
    *prevTickCount = curTickCount;
    return 1 / timeElapsed;
}

cv::Mat App::gui_filter_image(cv::Mat &raw) {
    cv::Mat output, grayscale;

    if (viewMode == ViewMode::NORMAL) {
        output = raw.clone();
        cv::resize(output, output, cv::Size(windowWidth, windowHeight));
        cv::cvtColor(output, output, cv::COLOR_BGR2RGB);
        return output;
    }


    int ksize = 3;
    int ksize_blur = 5;
    int delta = 1;
    int ddepth = CV_16S;

    cv::GaussianBlur(raw, output, cv::Size(ksize_blur, ksize_blur), 0);
    cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);
    cv::resize(output, output, cv::Size(windowWidth, windowHeight));

    cv::Mat grad_x, grad_y, grad;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(output, grad_x, ddepth, 1, 0, ksize, 1, delta, cv::BORDER_DEFAULT);
    cv::Sobel(output, grad_y, ddepth, 0, 1, ksize, 1, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    if (viewMode == ViewMode::DARK) {
        cv::bitwise_not(grad, grad);
    }

    cv::cvtColor(grad, output, cv::COLOR_GRAY2RGB);

    return output;
}

int App::exec() {
    cv::Mat frameImg, filtered;

    while (!isClosed) {

        try {
            frameImg = frameProvider->getFrame();
        } catch (std::exception &e) {
            break;
        }

        if (DEBUG_MODE)
            cv::imshow("Raw", frameImg);

        auto *sign = (SpeedLimitSign *) detector->detectRoadSign(frameImg);

        filtered = gui_filter_image(frameImg); // false for light mode

        QPixmap pixmap = QPixmap::fromImage(
                QImage((unsigned char *) filtered.data, filtered.cols, filtered.rows,
                       QImage::Format_RGB888)); //QImage::Format_BGR888
        framePreviewLabel->setPixmap(pixmap);
        paintedSignDrawer->setSpeedText(QString::fromStdString(std::to_string(sign->getLimit())));

        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();
    }
    return 0;
}

App::~App() {
    delete framePreviewLabel;
    delete muteButton;
    delete themeButton;
    delete layout;
    delete paintedSignDrawer;

}
