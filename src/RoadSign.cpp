//
// Created by maciej on 14.04.23.
//

#include "RoadSign.h"

RoadSign::RoadSign(SignType t, int v) {
    type = t;
    value = v;
}

SignType RoadSign::getType() const {
    return type;
}

int RoadSign::getValue() const {
    return value;
}

void RoadSign::setType(SignType t) {
    type = t;
}

void RoadSign::setValue(int v) {
    value = v;
}

