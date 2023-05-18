#include "../include/CircularRoadSignDetector.h"
#include "../include/Common.h"

RoadSign *CircularRoadSignDetector::detectRoadSign(cv::Mat &image) {
    auto *sign = (SpeedLimitSign *) roadSignDetectorEngine->detectRoadSign(image);
    buffer.push(sign->getLimit());
    int mostPopular = buffer.findMostPopularValue();

    if (mostPopular == 0 || mostPopular > 140)
        return lastSeenSign;

    sign->setLimit(mostPopular);
    if (sign->getLimit() == lastSeenSign->getLimit())
        return lastSeenSign;

    lastSeenSign = sign;
    callNotificationCallback();

    if (DEBUG_MODE) {
        std::cout << "Speed limit changed from " << lastSeenSign->getLimit() << " to " << sign->getLimit()
                  << std::endl;
    }
    return lastSeenSign;
}



CircularRoadSignDetector::~CircularRoadSignDetector() {
    delete roadSignDetectorEngine;
}

CircularRoadSignDetector::CircularRoadSignDetector(int bufferSize, IRoadSignDetector* engine): roadSignDetectorEngine(engine) {
    buffer = CircularBuffer<int>(bufferSize);
}
