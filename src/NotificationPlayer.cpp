#include "../include/NotificationPlayer.h"
#include <QMediaPlayer>
#include <QCoreApplication>

NotificationPlayer::NotificationPlayer(QString path) {
    player = new QMediaPlayer;
    player->setVolume(100);
    media = QUrl::fromLocalFile(path);

}

void NotificationPlayer::play() {
    if (player->state() == QMediaPlayer::PlayingState)
        player->setPosition(0);
    else if (player->state() == QMediaPlayer::StoppedState)
        player->setMedia(media);
    player->play();
}

void NotificationPlayer::changeVolume() {
    player->isMuted() ? player->setVolume(100) : player->setVolume(0);
}
