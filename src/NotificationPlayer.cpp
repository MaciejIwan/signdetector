#include "../include/NotificationPlayer.h"
#include <QMediaPlayer>
#include <QCoreApplication>

NotificationPlayer::NotificationPlayer(QString path) {
    player = new QMediaPlayer;
    unmute();
    media = QUrl::fromLocalFile(path);

}

void NotificationPlayer::play() {
    if (player->state() == QMediaPlayer::PlayingState)
        player->setPosition(0);
    else if (player->state() == QMediaPlayer::StoppedState)
        player->setMedia(media);
    player->play();
}

void NotificationPlayer::mute() {
    player->setVolume(0);
}

void NotificationPlayer::unmute() {
    player->setVolume(100);
}
