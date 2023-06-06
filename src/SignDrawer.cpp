#include "../include/SignDrawer.h"

void SignDrawer::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Set border color and width
    painter.setPen(QPen(Qt::black, 2));

    // Draw outer circle
    int outerCircleSize = 400 * 0.33;
    int outerCircleX = (400 - outerCircleSize) / 2;
    int outerCircleY = (600 - outerCircleSize+200) / 2;
    painter.drawEllipse(outerCircleX, outerCircleY, outerCircleSize, outerCircleSize);

    painter.setBrush(Qt::white);
    if(isDarkMode)
       painter.setBrush(Qt::lightGray);

    // Draw inner circle
    int innerCircleSize = 400 * 0.25;
    int innerCircleX = (400 - innerCircleSize) / 2;
    int innerCircleY = (600 - innerCircleSize+200) / 2;
    painter.drawEllipse(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize);

    QPainterPath ringPath;
    ringPath.addEllipse(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize);
    ringPath.addEllipse(outerCircleX, outerCircleY, outerCircleSize, outerCircleSize);

    // Set the fill color and fill the ring path
    painter.setBrush(Qt::red);
    if(isDarkMode)
        painter.setBrush(Qt::darkRed);

    painter.fillPath(ringPath, painter.brush());

    // Draw speed limit text
    QFont font("Arial", 36, QFont::Bold);
    painter.setFont(font);
    painter.drawText(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize, Qt::AlignCenter, speedLimit);

}

void SignDrawer::setThemeMode(bool theme){
    isDarkMode = theme;
    update();
}

void SignDrawer::setSpeedText(const QString &text) {
    speedLimit = text;
    update();
}
