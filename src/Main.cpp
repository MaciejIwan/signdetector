#include "../include/CircularRoadSignDetector.h"
#include "../include/App.h"
#include <string>

int main(int argc, char **argv) {
    std::string videoFile = "video/speed_limit_seqence_1.mp4";
    if (argc > 1)
        videoFile = std::string(argv[1]);

    auto *frameProvider = new FrameProvider(videoFile);
    auto *detector = new CircularRoadSignDetector();

    App uiApp(argc, argv, frameProvider, detector);

    return uiApp.exec();
}


