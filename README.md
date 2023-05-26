# Sign Detector

# About
This project is a software application that can recognize traffic signs in video streams. By leveraging OpenCV and Tesseract OCR, the program focuses on identifying speed limit signs and displaying them on the GUI to remind drivers to follow the speed limit.

# Project structure
- src/ - source files
- include/ - header files
- resources/ - imgs, videos for testing
- test/ - test files
- tools - helpful mini programs like:
    - Threshold.cpp - script for thresholding images and livetime preview

# Requirements
- OpenCV >=4.0
- CMake >=3.16
- g++ =>9.3.0
- tessaract >=4.0

### Install requirements on Ubuntu 20.04
```sh
sudo apt install build-essential
sudo apt-get install cmake
sudo apt install tesseract-ocr
sudo apt install libtesseract-dev
sudo apt install libopencv-dev
sudo apt-get install libmpg123-dev
sudo apt install qt5-default
sudo apt-get install qtmultimedia5-dev
```

# Build
### To build project to ./build directory run:
```sh
cmake -B build
```
### then go to build directory and run:
```sh
make
```

### then run main program like:
```sh
./main [path to video]
# f.e ./main video/znak1.mp4
```

# Usefull comands 
Here are some useful commands related to video processing that can be used in the project:
```sh
# Execute in no_resize directory
# This command resizes videos to 720p resolution (960x720 px).
for file in *.mp4; do ffmpeg -i "$file" -vf scale=-2:720 "../${file%.*}_720p.mp4"; done 


# Run video using ffplay
# Useful for checking the frame number when the sign is visible.
# To stop the video, press the spacebar.
ffplay -loop 0 speed_limit_night_12.mp4 -vf "drawtext=fontfile=Arial.ttf: \
text=%{n}: x=(w-tw)/2: y=h-(2*lh): \
fontcolor=white: fontsize=100: box=1: \
boxcolor=0x00000099"

```

### To run automated tests:
```sh
# in build directory
./tests
./tests --gtest_filter=RoiOcrTest* # to run example ts
./tests --gtest_filter=VideoTest* # to run tests for all videos
./tests --gtest_filter=VideoTest.speed_limit_night_19 # to run tests for video
```
