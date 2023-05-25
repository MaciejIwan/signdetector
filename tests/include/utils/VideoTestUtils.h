#pragma once

#include <string>

static const int delay = 1;
struct SignLabel {
    int frameStart;
    int frameEnd;
    int speedSignValue;
    bool detected;
};

int loadSignLabelsFromCSV(std::string filename, SignLabel *labels, int *labelCount);

void testSignRecognitionAccuracy(const std::string &filename);
