#pragma once

#include "RoadSign.h"

class SpeedLimitSign : public RoadSign {
private:
    int limit = DEFAULT_SPEED_LIMIT;
    static constexpr const int VALID_LIMITS[] = {10, 30, 40, 50, 60, 70, 90, 100, 110, 120, 130, 140};
public:
    static const int DEFAULT_SPEED_LIMIT = 0;

    SpeedLimitSign(int l) : RoadSign(SignType::SPEED_LIMIT) {
        setLimit(l);
    }

    friend std::ostream &operator<<(std::ostream &os, const SpeedLimitSign &speedLimit) {
        os << "Speed limit: " << speedLimit.limit;
        return os;
    }

    int getLimit() const {
        return limit;
    }

    void setLimit(int l) {
        bool valid_limit = false;
        for (const auto &lim: VALID_LIMITS) {
            if (l == lim) {
                valid_limit = true;
                break;
            }
        }
        if (valid_limit) {
            SpeedLimitSign::limit = l;
        } else {
            SpeedLimitSign::limit = DEFAULT_SPEED_LIMIT;
        }
    }

};
