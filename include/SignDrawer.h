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
    void setThemeMode(bool theme);
private:
    bool isDarkMode=true;
    QString speedLimit="0";

};


