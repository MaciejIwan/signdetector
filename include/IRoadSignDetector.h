#pragma once


#include <opencv2/core/mat.hpp>
#include "models/RoadSign.h"

using NotificationCallback = std::function<void()>;

class IRoadSignDetector {
public:
    virtual RoadSign *detectRoadSign(cv::Mat &image) = 0;

    void setNotificationCallback(const NotificationCallback &callback) {
        notificationCallback = callback;
    }



protected:
    NotificationCallback notificationCallback;

    void callNotificationCallback() {
        if (notificationCallback) {
            notificationCallback();
        }
    }
};