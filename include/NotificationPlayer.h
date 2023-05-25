#pragma once

#include <QMediaPlayer>

class NotificationPlayer {
public:
    explicit NotificationPlayer(QString path);
    void play();
    void mute();
    void unmute();
private:
    QMediaPlayer *player;
    QUrl media;
};

