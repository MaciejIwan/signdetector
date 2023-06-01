#include "../include/App.h"

App::App() {
    isDarkModeOn = true;
    isMuted = false;
    relativePath = "sound/notification_sound.mp3";
    notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);
    frame = new QLabel(&window);
    fpsLabel = new QLabel(&window);
    layout = new QVBoxLayout(&window);
    paintedSign = new SignDrawer(&window);
    muteButton = new QPushButton("Mute", &window);
    themeButton = new QPushButton("Dark mode", &window);
}

void App::init() {

    frame->setGeometry(0, 0, 600, 600);

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

void App::changeMode() {
    isDarkModeOn = !isDarkModeOn;
    isDarkModeOn ? themeButton->setText("Dark Mode") : themeButton->setText("Light Mode");
}

void App::changeVolume() {
    notificationPlayer.changeVolume();
    isMuted = !isMuted;
    isMuted ? muteButton->setText("Unmute") : muteButton->setText("Mute");
}
