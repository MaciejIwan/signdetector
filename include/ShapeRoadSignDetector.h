#pragma once

#include <opencv2/core/mat.hpp>
#include "Ocr.h"
#include <vector>
#include "models/RoadSign.h"
#include "IRoadSignDetector.h"

class ShapeRoadSignDetector : public IRoadSignDetector {
public:
    virtual RoadSign *detectRoadSign(cv::Mat &image) override;

    ShapeRoadSignDetector();

    ~ShapeRoadSignDetector();

private:
    const double MIN_CONTOUR_SIMILARITY = 0.75;
    Ocr ocr;

    static double compareContoursToCircle(const std::vector<cv::Point> &contour);

    cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);

    static void blurImage(cv::Mat &image, int size);

    void preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask);

    void findSignsBoundingBoxes(const cv::Mat &red_binary_mask, std::vector<cv::Rect> &boundingBoxes) const;
};

