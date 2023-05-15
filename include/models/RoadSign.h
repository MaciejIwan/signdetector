#pragma once

#include <string>

enum SignType {
    SPEED_LIMIT
};

class RoadSign {
protected:
    SignType type;

public:
    RoadSign(SignType t) {
        type = t;
    }

    SignType getType() const {
        return type;
    }

    void setType(SignType t) {
        type = t;
    }

};