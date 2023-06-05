#pragma once

#include <QMediaPlayer>

class NotificationPlayer {
public:
    explicit NotificationPlayer(QString path);
    void play();
    void changeVolume(bool isMuted) ;
private:
    QMediaPlayer *player;
    QUrl media;
};

