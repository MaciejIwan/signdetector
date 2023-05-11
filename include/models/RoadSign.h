//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H
#define SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H

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


#endif //SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H
