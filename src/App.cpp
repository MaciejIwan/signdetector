#include "../include/App.h"

App::App() {
    isDarkModeOn = true;
    isMuted = false;
    relativePath = "sound/notification_sound.mp3";
    notificationPlayer = NotificationPlayer(
            QCoreApplication::applicationDirPath() + "/" + relativePath);
    frame = new QLabel(&window);
    layout = new QVBoxLayout(&window);
    paintedSign = new SignDrawer(&window);
    muteButton = new QPushButton("Mute", &window);
    themeButton = new QPushButton("Light Mode", &window);
}

void App::init() {
    window.setFixedSize(400, 600);

    frame->setGeometry(0, 0, 600, 600);

    layout->addWidget(paintedSign);

    muteButton->setGeometry(30, 500, 150, 80);

    QObject::connect(muteButton, &QPushButton::clicked, [this]() {
        App::changeVolume();
    });

    themeButton->setGeometry(220, 500, 150, 80);
    QObject::connect(themeButton, &QPushButton::clicked, [this]() {
        App::changeMode();
    });

    themeButton->setStyleSheet("background-color: gray; color: white;");
    muteButton->setStyleSheet("background-color: gray; color: white;");

    window.setWindowTitle("Sign detector");
    window.setGeometry(400, 400, 400, 600);
    window.show();
}

void App::changeMode() {
    isDarkModeOn = !isDarkModeOn;
    if (isDarkModeOn) {
        themeButton->setText("Light Mode");
        themeButton->setStyleSheet("background-color: gray; color: white;");
        muteButton->setStyleSheet("background-color: gray; color: white;");
    } else {
        themeButton->setText("Dark Mode");
        themeButton->setStyleSheet("background-color: white; color: black;");
        muteButton->setStyleSheet("background-color: white; color: black;");
    }
    paintedSign->setThemeMode(isDarkModeOn);
}

void App::changeVolume() {
    isMuted = !isMuted;
    notificationPlayer.changeVolume(isMuted);
    isMuted ? muteButton->setText("Unmute") : muteButton->setText("Mute");
}
