#pragma once


#include <vector>
#include <opencv2/core/mat.hpp>
#include "Ocr.h"
#include "models/RoadSign.h"
#include "Common.h"
#include "models/SpeedLimitSign.h"

class IRoadSignDetector {
public:
    virtual RoadSign *detectRoadSign(cv::Mat &image) = 0;
};