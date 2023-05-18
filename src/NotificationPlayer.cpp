#include "../include/NotificationPlayer.h"

NotificationPlayer::NotificationPlayer() {}

void NotificationPlayer::play() {
    mpg123_handle *mpg123;
    ao_device *device;

    mpg123_init();
    mpg123 = mpg123_new(NULL, NULL);

    int channels, encoding;
    long rate;
    unsigned char *buffer;

    mpg123_open(mpg123, "/home/daniel/CLionProjects/sign_detector/resources/sounds/notification_sound.mp3");
    mpg123_getformat(mpg123, &rate, &channels, &encoding);
    ao_sample_format format;
    format.bits = mpg123_encsize(encoding);
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;

    device = ao_open_live(ao_default_driver_id(), &format, 0);

    size_t buffer_size = mpg123_outblock(mpg123);
    buffer = new unsigned char [buffer_size];
    size_t done;

    while (mpg123_read(mpg123, buffer, buffer_size, &done) == MPG123_OK)
        ao_play(device, reinterpret_cast<char*>(buffer), done);

    delete[] buffer;
    ao_close(device);
    mpg123_close(mpg123);
    mpg123_delete(mpg123);
    mpg123_exit();
    ao_shutdown();
}