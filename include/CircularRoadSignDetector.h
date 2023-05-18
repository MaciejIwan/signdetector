#pragma once

#include <opencv2/core/mat.hpp>
#include "models/RoadSign.h"
#include "IRoadSignDetector.h"
#include "CircularBuffer.h"
#include "models/SpeedLimitSign.h"

class CircularRoadSignDetector : public IRoadSignDetector {
public:
    RoadSign *detectRoadSign(cv::Mat &image) override;

    explicit CircularRoadSignDetector(int bufferSize = 30);

    ~CircularRoadSignDetector();

private:
    IRoadSignDetector *roadSignDetector;
    CircularBuffer<int> buffer;
    SpeedLimitSign *lastSeenSign = new SpeedLimitSign(SpeedLimitSign::DEFAULT_SPEED_LIMIT);
};
