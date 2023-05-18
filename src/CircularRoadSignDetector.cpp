#include "../include/CircularRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"
#include "../include/Common.h"

RoadSign *CircularRoadSignDetector::detectRoadSign(cv::Mat &image) {
    auto *sign = (SpeedLimitSign *) roadSignDetector->detectRoadSign(image);
    buffer.push(sign->getLimit());
    int mostPopular = buffer.findMostPopularValue();

    if (mostPopular == 0 || mostPopular > 140)
        return lastSeenSign;

    sign->setLimit(mostPopular);
    if (sign->getLimit() == lastSeenSign->getLimit())
        return lastSeenSign;

    lastSeenSign = sign;
    if (DEBUG_MODE) {
        std::cout << "Speed limit changed from " << lastSeenSign->getLimit() << " to " << sign->getLimit()
                  << std::endl;
    }
    return lastSeenSign;
}


CircularRoadSignDetector::~CircularRoadSignDetector() {
    delete roadSignDetector;
}

CircularRoadSignDetector::CircularRoadSignDetector(int bufferSize) {
    roadSignDetector = new ShapeRoadSignDetector();
    buffer = CircularBuffer<int>(bufferSize);
}
