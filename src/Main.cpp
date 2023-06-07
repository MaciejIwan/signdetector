#include "../include/CircularRoadSignDetector.h"
#include "../include/App.h"
#include <string>
#include <iostream>

void parseArgs(int argc, char **argv, FrameProviderConfig *config);
//-h 1080 -w 1920 -t camera -s 2
int main(int argc, char **argv) {
    FrameProviderConfig config = {
            1,
            FrameSourceType::VideoFile,
            "video/speed_limit_seqence_1.mp4",
            1280,
            720,
            30
    };

    parseArgs(argc, argv, &config);

    auto *frameProvider = new FrameProvider(config);
    auto *detector = new CircularRoadSignDetector();

    App uiApp(argc, argv, frameProvider, detector);

    int status = uiApp.exec();

    delete frameProvider;
    delete detector;

    return status;
}

void parseArgs(int argc, char **argv, FrameProviderConfig *config) {

    int option;
    while ((option = getopt(argc, argv, "h:w:s:t:b:")) != -1) {
        switch (option) {
            case 'h':
                config->height = std::stoi(optarg);
                break;
            case 'w':
                config->width = std::stoi(optarg);
                break;
            case 's':
                config->sourceType = FrameSourceType::LiveCamera;
                config->framesSource = optarg;
                break;
            case 't': {
                std::string optargS = std::string(optarg);
                if (optargS == "video")
                    config->sourceType = FrameSourceType::VideoFile;
                else if (optargS == "camera")
                    config->sourceType = FrameSourceType::LiveCamera;
                else {
                    std::cerr << "Invalid source type." << std::endl;
                    exit(1);
                }
                break;
            }
            case 'b':
                config->bufferSize = std::stoi(optarg);
                break;
            case 'r':
                config->frameRate = std::stoi(optarg);
                break;
            default:
                std::cerr << "Invalid option." << std::endl;
                exit(1);
        }
    }

    std::cout << "Buffer size: " << config->bufferSize << std::endl;
    std::cout << "Source type: " << static_cast<int>(config->sourceType) << std::endl;
    std::cout << "Frames source: " << config->framesSource << std::endl;
    std::cout << "Width: " << config->width << std::endl;
    std::cout << "Height: " << config->height << std::endl;
}
