//////
////// Created by maciej on 09.05.23.
//////
////


#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <dirent.h>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

void onTrackBar(int value, void *data) {
    cout << "Trackbar value: " << value << endl;

    auto *cap = static_cast<VideoCapture *>(data);

    cap->set(CAP_PROP_POS_FRAMES, value);

    Mat frame;
    cap->read(frame);

    imshow("Video Player", frame);

}

struct d {
    vector<string> *videoFileNames;
    VideoCapture *cap;
    int *currentFrame;
    int *numFrames;
};

void onVideoBar(int value, void *data) {
    //cout << "Trackbar value video bar: " << value << endl;
    auto *currentVideoData = static_cast<d *>(data);

    currentVideoData->cap->release();

    *(currentVideoData->cap) = VideoCapture((*currentVideoData->videoFileNames)[value]);
    *(currentVideoData->numFrames) = currentVideoData->cap->get(CAP_PROP_FRAME_COUNT);
    *(currentVideoData->currentFrame) = 0;

    Mat frame;
    currentVideoData->cap->read(frame);


    setTrackbarMax("Frame", "Video Player", *(currentVideoData->numFrames) - 1);
    setTrackbarPos("Frame", "Video Player", 0);

    imshow("Video Player", frame);

    string filename = (*currentVideoData->videoFileNames)[value];
    int lastSlashIndex = filename.find_last_of("/");
    string videoName = filename.substr(lastSlashIndex + 1);
    setWindowTitle("Video Player", videoName);
}

int frameNumber = 0;
int currentFrame = 0;
int numFrames = 0;

int readVideoFilesInFolder(string folderPath, vector<string> *videoFileNames) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folderPath.c_str())) == NULL) {
        cout << "Error: Failed to open folder" << endl;
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        string fileName = ent->d_name;
        if (fileName.find(".avi") != string::npos ||
            fileName.find(".mp4") != string::npos ||
            fileName.find(".mov") != string::npos) {
            videoFileNames->push_back(folderPath + fileName);
        }
    }

    closedir(dir);

    return 0;
}

//todo display filename in window
//todo allow to mark frames (start, end) and save them to .csv label file
int main(int argc, char **argv) {
    string folderPath = "../resources/video/";

    vector<string> videoFileNames;

    if (readVideoFilesInFolder(folderPath, &videoFileNames))
        return -1;

    namedWindow("Video Player", WINDOW_NORMAL);


    int videoIndex = 0;
    string videoFileName = videoFileNames[videoIndex];

    VideoCapture videoCapture(videoFileName);

    numFrames = videoCapture.get(CAP_PROP_FRAME_COUNT);

    struct d currentVideoData;
    currentVideoData.cap = &videoCapture;
    currentVideoData.videoFileNames = &videoFileNames;
    currentVideoData.currentFrame = &currentFrame;
    currentVideoData.numFrames = &numFrames;

    createTrackbar("Frame", "Video Player", &frameNumber, numFrames + 1, onTrackBar, &videoCapture);
    createTrackbar("Video", "Video Player", &videoIndex, videoFileNames.size() - 1, onVideoBar, &currentVideoData);

    setTrackbarMax("Frame", "Video Player", numFrames - 1);

    int lastSlashIndex = videoFileName.find_last_of("/");
    string videoName = videoFileName.substr(lastSlashIndex + 1);
    setWindowTitle("Video Player", videoName);

    Mat frame;
    videoCapture >> frame;
    imshow("Video Player", frame);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

