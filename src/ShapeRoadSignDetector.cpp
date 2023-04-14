//
// Created by maciej on 14.04.23.
//

#include <opencv2/imgproc.hpp>
#include "IRoadSignDetector.h"

class ShapeRoadSignDetector : public IRoadSignDetector {
public:
    RoadSign detectRoadSigns(const cv::Mat &image) override {
        RoadSign roadSign = RoadSign(SignType::SPEED_LIMIT, 30);


        return roadSign;
    }

private:
    void blurImage(cv::Mat &image, int size, double sigma) {
        cv::Size kernelSize = cv::Size(size, size);
        //cv::GaussianBlur(image, image, kernelSize, sigma);
        cv::medianBlur(image, image, size);
    }

};