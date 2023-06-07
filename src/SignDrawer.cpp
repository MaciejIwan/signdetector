#include "../include/SignDrawer.h"

void SignDrawer::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Set border color and width
    painter.setPen(QPen(Qt::black, 2));

    // Draw outer circle
    int outerCircleSize = signSize-30;
    int outerCircleX = (width() - outerCircleSize) / 2;
    int outerCircleY = (height() - outerCircleSize+300) / 2;
    painter.drawEllipse(outerCircleX, outerCircleY, outerCircleSize, outerCircleSize);

    painter.setBrush(Qt::white);
    if(isDarkMode)
       painter.setBrush(Qt::lightGray);

    // Draw inner circle
    int innerCircleSize = signSize;
    int innerCircleX = (width() - innerCircleSize) / 2;
    int innerCircleY = (height() - innerCircleSize+300) / 2;
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
