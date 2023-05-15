#pragma once


#include <opencv2/core/mat.hpp>
#include "models/RoadSign.h"

class IRoadSignDetector {
public:
    virtual RoadSign *detectRoadSign(cv::Mat &image) = 0;
};