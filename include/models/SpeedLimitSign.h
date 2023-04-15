//
// Created by maciej on 15.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H
#define SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H


#include "RoadSign.h"

class SpeedLimitSign : public RoadSign {
private:
    int limit;

public:
    SpeedLimitSign(int l) : RoadSign(SignType::SPEED_LIMIT), limit(l) {}

    std::string toString() override {
        return "Speed limit: " + std::to_string(limit);
    }

    int getLimit() const {
        return limit;
    }

    void setLimit(int limit) {
        SpeedLimitSign::limit = limit;
    }
};

#endif //SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H
