#pragma once

#include <QApplication>
#include <QAudioOutput>
#include <QFont>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include "../include/NotificationPlayer.h"
#include "../include/SignDrawer.h"
#include "FrameProvider.h"

enum ViewMode {
    DARK,
    LIGHT,
    NORMAL
};
class App : QApplication {
public:
    App(int &argc, char **argv, FrameProvider *frameProvider, IRoadSignDetector *detector);
    ViewMode viewMode;
    bool isMuted;

    ~App() override;

    bool isClosed;
    NotificationPlayer notificationPlayer = NotificationPlayer(QString());
    QString relativePath;
    QWidget window;
    QLabel *frameLabel;
    QPushButton *muteButton;
    QPushButton *themeButton;
    QVBoxLayout *layout;
    SignDrawer *paintedSignDrawer;


    App * init();
    void changeMode();
    void changeVolume();
    int exec();

    cv::Mat gui_filter_image(cv::Mat &raw);

private:
    IRoadSignDetector* detector;
    FrameProvider* frameProvider;
};

