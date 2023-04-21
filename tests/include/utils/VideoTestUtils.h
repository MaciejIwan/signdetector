//
// Created by maciej on 19.04.23.
//

#ifndef SIGN_EDGE_METHOD_TEST_1_VIDEOTESTUTILS_H
#define SIGN_EDGE_METHOD_TEST_1_VIDEOTESTUTILS_H

#include <string>

static const int delay = 1;
struct SignLabel {
    int frameStart;
    int frameEnd;
    int speedSignValue;
    bool detected;
};

int loadSignLabelsFromCSV(std::string filename, SignLabel* labels, int* labelCount);
void testSignRecognitionAccuracy(const std::string& filename);
#endif // SIGN_EDGE_METHOD_TEST_1_VIDEOTESTUTILS_H
