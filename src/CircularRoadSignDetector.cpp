//
// Created by maciej on 15.05.23.
//

#include "../include/CircularRoadSignDetector.h"
#include "../include/ShapeRoadSignDetector.h"

RoadSign *CircularRoadSignDetector::detectRoadSign(cv::Mat &image) {
    return roadSignDetector->detectRoadSign(image);
}

CircularRoadSignDetector::CircularRoadSignDetector() {
    roadSignDetector = new ShapeRoadSignDetector();
}

CircularRoadSignDetector::~CircularRoadSignDetector() {
    delete roadSignDetector;
}
