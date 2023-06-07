#pragma once

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QFont>

class SignDrawer :  public QWidget {
public:
    explicit SignDrawer(int size, QWidget *parent = nullptr) : QWidget(parent), signSize(size) {}
    void paintEvent(QPaintEvent *event) override;
    void setSpeedText(const QString& text);
    void setThemeMode(bool theme);
private:
    bool isDarkMode=false;
    QString speedLimit="0";
    int signSize;

};


