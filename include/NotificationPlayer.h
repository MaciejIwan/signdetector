#pragma once

#include <QMediaPlayer>

class NotificationPlayer {
public:
    explicit NotificationPlayer(QString path);
    void play();
    void changeVolume() ;
private:
    QMediaPlayer *player;
    QUrl media;
};

