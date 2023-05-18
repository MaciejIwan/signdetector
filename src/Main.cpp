#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/NotificationPlayer.h"
#include "../include/CircularBuffer.h"
#include "../include/models/SpeedLimitSign.h"
#include "../include/Common.h"
#include "../include/CircularRoadSignDetector.h"

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QAudioOutput>

float calcFPS(int64 *prevTickCount);

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    const QString relativePath = "sound/notification_sound.mp3";
    NotificationPlayer notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);

    QWidget window;
    QLabel *label = new QLabel(&window);
    label->setGeometry(0, 0, 600, 600);

    QLabel *speedLimitLabel = new QLabel(&window);
    QLabel *fpsLabel = new QLabel(&window);

    QPushButton *button = new QPushButton("Wycisz", &window);

    speedLimitLabel->setGeometry(30, 100, 100, 30);
    QFont font = speedLimitLabel->font();
    font.setPointSize(28);
    font.setBold(true);
    speedLimitLabel->setFont(font);
    speedLimitLabel->setNum(0);

    fpsLabel->setGeometry(30, 150, 100, 30);
    font = fpsLabel->font();
    font.setPointSize(18);
    font.setBold(false);
    fpsLabel->setFont(font);
    fpsLabel->setText("fps: " + QString::number(0));

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, 600, 600);
    window.show();

    IRoadSignDetector *detector = new CircularRoadSignDetector();

    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    cv::VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video file " << std::endl;
        return -1;
    }

    cv::Mat frame;
    int64 prevTickCount = cv::getTickCount();

    while (true) {
        if (!cap.read(frame)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }
        QPixmap pixmap = QPixmap::fromImage(
                QImage((unsigned char *) frame.data, frame.cols, frame.rows, QImage::Format_BGR888));


        auto *sign = (SpeedLimitSign *) detector->detectRoadSign(frame);

        label->setPixmap(pixmap);

        fpsLabel->setText("fps: " + QString::number(calcFPS(&prevTickCount)));
        speedLimitLabel->setNum(sign->getLimit());
        notificationPlayer.play();

        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();

    }

    return app.exec();
}

float calcFPS(int64 *prevTickCount) {
    int64 curTickCount = cv::getTickCount();
    double timeElapsed = (double)(curTickCount - *prevTickCount) / cv::getTickFrequency();
    *prevTickCount = curTickCount;
    return 1 / timeElapsed;
}


