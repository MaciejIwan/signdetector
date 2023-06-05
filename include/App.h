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

class App {
public:
    bool isDarkModeOn;
    bool isMuted;
    NotificationPlayer notificationPlayer = NotificationPlayer(QString());
    QString relativePath;
    QWidget window;
    QLabel *frame;
    QPushButton *muteButton;
    QPushButton *themeButton;
    QVBoxLayout *layout;
    SignDrawer *paintedSign;

    explicit App();
    void init();
    void changeMode();
    void changeVolume();

};

