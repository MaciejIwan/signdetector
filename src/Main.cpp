#include "../include/IRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/NotificationPlayer.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include "../include/CircularBuffer.h"
#include "../include/models/SpeedLimitSign.h"
#include "../include/Common.h"
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QAudioOutput>

int main(int argc, char **argv) {
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;

    QApplication app(argc, argv);

    const QString relativePath = "sound/notification_sound.mp3";
    NotificationPlayer notificationPlayer = NotificationPlayer(QCoreApplication::applicationDirPath() + "/" + relativePath);

    QWidget window;
    QLabel *label = new QLabel(&window);
    QLabel *speedLimit = new QLabel(&window);

    QPushButton *button = new QPushButton("Wycisz", &window);

    speedLimit->setGeometry(30, 100, 100, 30);

    QFont font = speedLimit->font();
    font.setPointSize(28);
    font.setBold(true);
    speedLimit->setFont(font);
    speedLimit->setNum(0);

    label->setGeometry(0, 0, 600, 600);

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, 600, 600);
    window.show();

    ShapeRoadSignDetector detector = ShapeRoadSignDetector();
    auto *lastSeenSign = new SpeedLimitSign(SpeedLimitSign::DEFAULT_SPEED_LIMIT);

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
    double fps = 0;

    // TODO: it should keep Signs not ints
    CircularBuffer<int> buffer(30);
    while (true) {
        if (!cap.read(frame)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char *) frame.data,
                                                   frame.cols,
                                                   frame.rows,
                                                   QImage::Format_BGR888));
        label->setPixmap(pixmap);
        int64 curTickCount = cv::getTickCount();
        double timeElapsed = (curTickCount - prevTickCount) / cv::getTickFrequency();
        prevTickCount = curTickCount;
        fps = 1 / timeElapsed;

        auto *sign = (SpeedLimitSign *) detector.detectRoadSign(frame);

        // todo integrate buffer with system (maybe decetor module). Now it is just print value to console
        buffer.push(sign->getLimit());
        int mostPopular = buffer.findMostPopularValue();
        if (mostPopular != 0 && mostPopular <= 140) {
            sign->setLimit(mostPopular);
            if (sign->getLimit() != lastSeenSign->getLimit()) {
                std::cout << "Speed limit changed from " << lastSeenSign->getLimit() << " to " << sign->getLimit()
                          << std::endl;
                lastSeenSign = sign;
                speedLimit->setNum(sign->getLimit());
                notificationPlayer.play();
            }
        }
        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();

    }
    delete lastSeenSign;
    return app.exec();
}


