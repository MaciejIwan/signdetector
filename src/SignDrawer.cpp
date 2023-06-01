#include "../include/SignDrawer.h"

void SignDrawer::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Set border color and width
    painter.setPen(QPen(Qt::black, 2));

    // Draw outer circle
    int outerCircleSize = width() * 0.33;
    int outerCircleX = (width() - outerCircleSize) / 2;
    int outerCircleY = (height() - outerCircleSize+200) / 2;
    painter.drawEllipse(outerCircleX, outerCircleY, outerCircleSize, outerCircleSize);

    // Draw inner circle
    int innerCircleSize = width() * 0.25;
    int innerCircleX = (width() - innerCircleSize) / 2;
    int innerCircleY = (height() - innerCircleSize+200) / 2;
    painter.drawEllipse(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize);

    QPainterPath ringPath;
    ringPath.addEllipse(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize);
    ringPath.addEllipse(outerCircleX, outerCircleY, outerCircleSize, outerCircleSize);

    // Set the fill color and fill the ring path
    painter.setBrush(Qt::red);
    painter.fillPath(ringPath, painter.brush());

    // Draw speed limit text
    QFont font("Arial", 36, QFont::Bold);
    painter.setFont(font);
    painter.drawText(innerCircleX, innerCircleY, innerCircleSize, innerCircleSize, Qt::AlignCenter, speedLimit);

}

void SignDrawer::setSpeedText(const QString &text) {
    speedLimit = text;
    update();
}
