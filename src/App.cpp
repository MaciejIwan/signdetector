#include "../include/App.h"

App::App() {
    isDarkModeOn = true;
    isMuted = false;
    relativePath = "sound/notification_sound.mp3";
    notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);
    backgroundFrame = new QLabel(&window);
    fpsLabel = new QLabel(&window);
    layout = new QVBoxLayout(&window);
    paintedSign = new SignDrawer(&window);
    muteButton = new QPushButton("Mute", &window);
    themeButton = new QPushButton("Dark mode", &window);
    detector = new CircularRoadSignDetector();

}

void App::init() {


    backgroundFrame->setGeometry(0, 0, 600, 600);

    layout->addWidget(paintedSign);

    fpsLabel->setGeometry(30, 150, 100, 30);

    QFont font = fpsLabel->font();
    font.setPointSize(18);
    font.setBold(false);
    fpsLabel->setFont(font);
    fpsLabel->setText("fps: " + QString::number(0));

    muteButton->setGeometry(30, 500, 150, 80);
    QObject::connect(muteButton, &QPushButton::clicked, [this]() {
        App::changeVolume();
    });

    themeButton->setGeometry(220, 500, 150, 80);
    QObject::connect(themeButton, &QPushButton::clicked, [this]() {
        App::changeMode();
    });

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, 400, 600);
    window.show();
}

int App::start(int argc, char** argv){

    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc == 2)
        videoFile = std::string(argv[1]);

    init();

    FrameProvider frameProvider = FrameProvider(videoFile); // is it checking if file exists?

    detector->setNotificationCallback([this]() {
        notificationPlayer.play();
    });
    cv::Mat frame, filtered;
    int64 prevTickCount = cv::getTickCount();

    while (true) {

        frame = frameProvider.getFrame();
        auto* sign = (SpeedLimitSign*)detector->detectRoadSign(frame);

        filtered = gui_filter_image(frame, isDarkModeOn); // false for light mode

        QPixmap pixmap = QPixmap::fromImage(
                QImage((unsigned char*)filtered.data, filtered.cols, filtered.rows, QImage::Format_BGR888));
        backgroundFrame->setPixmap(pixmap);
        fpsLabel->setText("fps: " + QString::number(calcFPS(&prevTickCount)));
        paintedSign->setSpeedText(QString::fromStdString(std::to_string(sign->getLimit())));

        if (sign->getLimit() != 0 && DEBUG_MODE) {
            std::cout << *sign << std::endl;
        }

        QCoreApplication::processEvents();
    }
    return 0;
}

void App::changeMode() {
    isDarkModeOn = !isDarkModeOn;
    isDarkModeOn ? themeButton->setText("Dark Mode") : themeButton->setText("Light Mode");
}

void App::changeVolume() {
    notificationPlayer.changeVolume();
    isMuted = !isMuted;
    isMuted ? muteButton->setText("Unmute") : muteButton->setText("Mute");
}

float App::calcFPS(int64* prevTickCount){
    int64 curTickCount = cv::getTickCount();
    double timeElapsed = (double)(curTickCount - *prevTickCount) / cv::getTickFrequency();
    *prevTickCount = curTickCount;
    return 1 / timeElapsed;
}

cv::Mat App::gui_filter_image(cv::Mat& raw, bool darkmode)
{
    // Applies simple Sobel edge detection to simplify
    // the raw image and make it more readable for GUI

    int ksize = 3;
    int ksize_blur = 5;
    int delta = 1;
    int ddepth = CV_16S;

    cv::Mat output, grayscale;

    cv::GaussianBlur(raw, output, cv::Size(ksize_blur, ksize_blur), 0);
    cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y, grad;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(output, grad_x, ddepth, 1, 0, ksize, 1, delta, cv::BORDER_DEFAULT);
    cv::Sobel(output, grad_y, ddepth, 0, 1, ksize, 1, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    if (!darkmode) {
        cv::bitwise_not(grad, grad);
    }

    cv::cvtColor(grad, output, cv::COLOR_GRAY2RGB);

    return output;
}