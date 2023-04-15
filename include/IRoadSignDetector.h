//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H
#define SIGN_EDGE_METHOD_TEST_1_IROADSIGNDETECTOR_H


#include <vector>
#include <opencv2/core/mat.hpp>
#include "Ocr.h"
#include "models/RoadSign.h"

class IRoadSignDetector {
public:
    virtual RoadSign* detectRoadSigns(const cv::Mat &image) = 0;
};


class ShapeRoadSignDetector : public IRoadSignDetector {
public:
    RoadSign* detectRoadSigns(const cv::Mat &image) override;

    ShapeRoadSignDetector();

    ~ShapeRoadSignDetector();

    void updateImageView(cv::Mat &currentFrame,
                         int &lastSpeedLimit); // todo Split and simplify this function. Then remove it and use detectRoadSigns instead. Extract: preprocessing, contour detection, selection, recognition etc

private:
    const double MIN_CONTOUR_SIMILARITY = 0.75;
    Ocr myOcr;

    static double compareContoursToCircle(const std::vector<cv::Point> &contour);

    cv::Mat extractRedColorFromImage(const cv::Mat &hsvFrame);


    static void blurImage(cv::Mat &image, int size);

    void preprocess(cv::Mat &currentFrame, cv::Mat &red_binary_mask);

    void findSignsBoundingBoxes(const cv::Mat &red_binary_mask, std::vector<cv::Rect> &boundingBoxes) const;

    void drawSpeedLimitOnFrame(const cv::Mat &currentFrame, const int &lastSpeedLimit) const;
};

#endif
