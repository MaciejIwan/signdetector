#include "../include/CircularRoadSignDetector.h"
#include "../include/Common.h"

RoadSign *CircularRoadSignDetector::detectRoadSign(cv::Mat &image) {
    auto *sign = (SpeedLimitSign *) roadSignDetectorEngine->detectRoadSign(image);
    buffer.push(sign->getLimit());
    int mostPopular = buffer.findMostPopularValue();

    if (mostPopular == 0 || mostPopular > 140)
        return lastSeenSign;

    sign->setLimit(mostPopular);
    if (sign->getLimit() == lastSeenSign->getLimit()){
        delete sign;
        return lastSeenSign;
    }

    delete lastSeenSign;
    lastSeenSign = sign;
    callNotificationCallback();

    if (DEBUG_MODE) {
        std::cout << "Speed limit changed from " << lastSeenSign->getLimit() << " to " << sign->getLimit()
                  << std::endl;
    }
    return lastSeenSign;
}



CircularRoadSignDetector::~CircularRoadSignDetector() {
    std::cout << "CircularRoadSignDetector destructor" << std::endl;
    roadSignDetectorEngine->~IRoadSignDetector();
    delete lastSeenSign;
}

CircularRoadSignDetector::CircularRoadSignDetector(int bufferSize, IRoadSignDetector* engine): roadSignDetectorEngine(engine) {
    buffer = CircularBuffer<int>(bufferSize);
}
