#include <gtest/gtest.h>
#include "../include/NotificationPlayer.h"
#include <ao/ao.h>
#include <mpg123.h>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QDebug>
#include <QCoreApplication>


TEST(NotificationPlayerTest, shouldReturn30_1) {
    NotificationPlayer notificationPlayer = NotificationPlayer();
    notificationPlayer.play();


    ASSERT_EQ(true, true);
}
TEST(QtNotificationPlayerTest, shouldReturn30_1) {
    QString path = "sound/notification_sound.mp3";
    QMediaPlayer *mediaPlayer = new QMediaPlayer();
    mediaPlayer->setVolume(50);
    mediaPlayer->setMedia(QUrl::fromLocalFile(path));
    mediaPlayer->setPosition(0);

    QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged, [&](qint64 position){
        qDebug() << "Current position:" << position / 1000 << "seconds";
    });

//    QTimer timer;
//    QObject::connect(&timer, &QTimer::timeout, [&](){
//        qint64 position = mediaPlayer->position();
//        qDebug() << "Current position:" << position / 1000 << "seconds";
//    });
//    timer.start(1000);
    mediaPlayer->play();

    ASSERT_EQ(false, false);
}

TEST(NotificationPlayerTest2, shouldReturn30_1) {
      std::cout<<'\a';

//    mpg123_handle *mh;
//    unsigned char *buffer;
//    size_t buffer_size;
//    size_t done;
//    int err;
//
//    int driver;
//    ao_device *dev;
//
//    ao_sample_format format;
//    int channels, encoding;
//    long rate;
//
//
//    /* initializations */
//    ao_initialize();
//    driver = ao_default_driver_id();
//    mpg123_init();
//    mh = mpg123_new(NULL, &err);
//    buffer_size = mpg123_outblock(mh);
//    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
//
//    /* open the file and get the decoding format */
//    mpg123_open(mh, "sound/notification_sound.mp3");
//    mpg123_getformat(mh, &rate, &channels, &encoding);
//
//    /* set the output format and open the output device */
//    format.bits = mpg123_encsize(encoding) * 8;
//    format.rate = rate;
//    format.channels = channels;
//    format.byte_format = AO_FMT_NATIVE;
//    format.matrix = 0;
//    dev = ao_open_live(driver, &format, NULL);
//
//    /* decode and play */
//    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
//        ao_play(dev, reinterpret_cast<char*>(buffer), done);
//
//    /* clean up */
//    free(buffer);
//    ao_close(dev);
//    mpg123_close(mh);
//    mpg123_delete(mh);
//    mpg123_exit();
//    ao_shutdown();

    ASSERT_EQ(true, true);
}





