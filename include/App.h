#pragma once

#include "../include/CircularBuffer.h"
#include "../include/CircularRoadSignDetector.h"
#include "../include/Common.h"
#include "../include/FrameProvider.h"
#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"
#include "../include/NotificationPlayer.h"
#include "../include/SignDrawer.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <string>
#include <QApplication>
#include <QAudioOutput>
#include <QFont>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>


class App {
public:
    bool isDarkModeOn;
    bool isMuted;
    NotificationPlayer notificationPlayer = NotificationPlayer(QString());
    QString relativePath;
    QWidget window;
    QLabel *backgroundFrame;
    QPushButton *muteButton;
    QPushButton *themeButton;
    QVBoxLayout *layout;
    QLabel *fpsLabel;
    SignDrawer *paintedSign;
    IRoadSignDetector* detector;




    explicit App();
    void init();
    void changeMode();
    void changeVolume();
    int start(int argc, char** argv);
    float calcFPS(int64* prevTickCount);
    cv::Mat gui_filter_image(cv::Mat& raw, bool darkmode);

};

