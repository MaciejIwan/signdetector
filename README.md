To build project to ./build directory, run:
```sh
cmake -B build
```

then go to build directory and run:
```sh
make
```

then run main program like:
```sh
./main [path to video]
# f.e ./main video/znak1.mp4
```

usefull comands 
```sh
# excecute in no_resize dir
for file in *.mp4; do ffmpeg -i "$file" -vf scale=-2:720 "../${file%.*}_720p.mp4"; done # resize videos to 720p (1280x720 px)
```