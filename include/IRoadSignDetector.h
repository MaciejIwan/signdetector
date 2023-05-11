//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H
#define SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H


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

#endif
