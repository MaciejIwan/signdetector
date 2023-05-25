#pragma once

#include "IRoadSignDetector.h"
#include "models/RoadSign.h"
#include <opencv2/core/mat.hpp>
#include "ShapeRoadSignDetector.h"
#include "CircularBuffer.h"
#include "models/SpeedLimitSign.h"



class CircularRoadSignDetector : public IRoadSignDetector {
public:
    explicit CircularRoadSignDetector(int bufferSize = 30, IRoadSignDetector *engine = new ShapeRoadSignDetector());

    RoadSign *detectRoadSign(cv::Mat &image) override;

    ~CircularRoadSignDetector();

private:
    IRoadSignDetector *roadSignDetectorEngine;
    CircularBuffer<int> buffer;
    SpeedLimitSign *lastSeenSign = new SpeedLimitSign(SpeedLimitSign::DEFAULT_SPEED_LIMIT);
};
