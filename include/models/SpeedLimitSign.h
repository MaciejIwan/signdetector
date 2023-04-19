//
// Created by maciej on 15.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H
#define SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H

#include "RoadSign.h"

class SpeedLimitSign : public RoadSign {
private:
    int limit = DEFAULT_SPEED_LIMIT;
    static constexpr const int VALID_LIMITS[] = {10, 30, 40, 50, 60, 70, 90, 100, 110, 120, 130, 140};
public:
    static const int DEFAULT_SPEED_LIMIT = 0;
    //SpeedLimitSign(int l) : RoadSign(SignType::SPEED_LIMIT), limit(l) {}
    SpeedLimitSign(int l) : RoadSign(SignType::SPEED_LIMIT) {
        setLimit(l);
    }

    std::string toString() override {
        return "Speed limit: " + std::to_string(limit);
    }

    int getLimit() const {
        return limit;
    }

    void setLimit(int l) {
        limit = l;
    }
    void updateLastSeenSign(int value) {
        bool valid_limit = false;
        for (const auto &lim: VALID_LIMITS) {
            if (value == lim) {
                valid_limit = true;
                break;
            }
        }
        if (valid_limit) {
            SpeedLimitSign::limit = value;
        }
    }
};

#endif //SIGN_EDGE_METHOD_TEST_1_SPEEDLIMITSIGN_H
