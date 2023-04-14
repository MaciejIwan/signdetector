//
// Created by maciej on 14.04.23.
//

#include "IRoadSignDetector.h"

class ShapeRoadSignDetector : public IRoadSignDetector {
public:
    std::vector<RoadSign> detectRoadSigns(const cv::Mat& image) override {
        std::vector<RoadSign> roadSigns;


        return roadSigns;
    }
};