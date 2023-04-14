//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H
#define SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H


#include <vector>
#include <opencv2/core/mat.hpp>
#include "RoadSign.h"

class IRoadSignDetector {
public:
    virtual std::vector<RoadSign> detectRoadSigns(const cv::Mat& image) = 0;
};

#endif
