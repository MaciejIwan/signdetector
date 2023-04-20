//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H
#define SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H


#include <vector>
#include <opencv2/core/mat.hpp>
#include "Ocr.h"
#include "models/RoadSign.h"
#include "common.h"
#include "models/SpeedLimitSign.h"

class IRoadSignDetector {
public:
    virtual RoadSign *detectRoadSign(cv::Mat &image) = 0;
};


class ShapeRoadSignDetector : public IRoadSignDetector {
public:
    RoadSign *detectRoadSign(cv::Mat &image) override;

    ShapeRoadSignDetector();

    ~ShapeRoadSignDetector();

private:
    const double MIN_CONTOUR_SIMILARITY = 0.75;
    Ocr ocr;
    int lastSpeedLimit = 0;
    SpeedLimitSign lastSeenSign = SpeedLimitSign(SpeedLimitSign::DEFAULT_SPEED_LIMIT);

    static double compareContoursToCircle(const std::vector<cv::Point> &contour);

    cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);

    static void blurImage(cv::Mat &image, int size);

    void preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask);

    void findSignsBoundingBoxes(const cv::Mat &red_binary_mask, std::vector<cv::Rect> &boundingBoxes) const;

    void drawSpeedLimitOnFrame(const cv::Mat &currentFrame) const;
};

#endif
