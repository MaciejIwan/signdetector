#include "../include/CircularBuffer.h"
#include "../include/CircularRoadSignDetector.h"
#include "../include/Common.h"
#include "../include/FrameProvider.h"
#include "../include/IRoadSignDetector.h"
#include "../include/App.h"
#include "../include/ShapeRoadSignDetector.h"
#include "../include/models/SpeedLimitSign.h"


#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <string>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    App uiApp;
    uiApp.start(argc, argv);

    return app.exec();
}

