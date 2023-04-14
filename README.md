## To build project to ./build directory, run:
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

## usefull comands 
```sh
# excecute in no_resize dir
for file in *.mp4; do ffmpeg -i "$file" -vf scale=-2:720 "../${file%.*}_720p.mp4"; done # resize videos to 720p (1280x720 px)
```

# Requirements
- OpenCV 4.5.1
- CMake 3.20.2
- C++17
- g++ 9.3.0
- tessaract 

# Installation
```sh
sudo apt install build-essential
sudo apt-get install cmake
sudo apt install tesseract-ocr
sudo apt install libtesseract-dev
sudo apt install libopencv-dev
```