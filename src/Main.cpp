#include "../include/CircularRoadSignDetector.h"
#include "../include/App.h"
#include <string>

int main(int argc, char **argv) {
    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc > 1)
        videoFile = std::string(argv[1]);

    int bufferSize = 1;

    auto *frameProvider = new FrameProvider(videoFile, bufferSize, FrameSourceType::VideoFile);
    auto *detector = new CircularRoadSignDetector();

    App uiApp(argc, argv, frameProvider, detector);

    int status = uiApp.exec();

    delete frameProvider;
    delete detector;

    return status;
}


