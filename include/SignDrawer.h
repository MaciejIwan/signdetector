#pragma once

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QFont>

class SignDrawer :  public QWidget {
public:
    explicit SignDrawer(QWidget *parent = nullptr) : QWidget(parent) {}
    void paintEvent(QPaintEvent *event) override;
    void setSpeedText(const QString& text);
private:
    QString speedLimit;

};


