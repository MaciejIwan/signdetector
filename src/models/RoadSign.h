//
// Created by maciej on 14.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H
#define SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H

enum SignType {
    SPEED_LIMIT,
    WARNING,
    REGULATORY
};

class RoadSign {
private:
    SignType type;
    int value;

public:
    RoadSign(SignType t, int v) {
        type = t;
        value = v;
    }

    SignType getType() const {
        return type;
    }

    int getValue() const {
        return value;
    }

    void setType(SignType t) {
        type = t;
    }

    void setValue(int v) {
        value = v;
    }

};


#endif //SIGN_EDGE_METHOD_TEST_1_ROADSIGN_H
