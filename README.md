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
```sh
# excecute in no_resize dir
for file in *.mp4; do ffmpeg -i "$file" -vf scale=-2:720 "../${file%.*}_720p.mp4"; done # resize videos to 720p (1280x720 px)
```

