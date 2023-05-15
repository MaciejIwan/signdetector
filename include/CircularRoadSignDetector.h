//
// Created by maciej on 15.05.23.
//

#pragma once

#include <opencv2/core/mat.hpp>
#include "models/RoadSign.h"
#include "IRoadSignDetector.h"

class CircularRoadSignDetector : public IRoadSignDetector {
public:
    RoadSign *detectRoadSign(cv::Mat &image) override;

    CircularRoadSignDetector();

    ~CircularRoadSignDetector();

private:
    IRoadSignDetector *roadSignDetector;
};