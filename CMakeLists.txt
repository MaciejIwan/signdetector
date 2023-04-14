cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(sign_edge_method_test_1)

find_package(OpenCV 4.0 REQUIRED)
find_package(Tesseract 4)
if (NOT Tesseract_FOUND)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET tesseract lept)
    add_library(Tesseract::libtesseract ALIAS PkgConfig::Tesseract)
endif ()

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
)

add_executable(main src/Main.cpp src/models/RoadSign.cpp src/models/RoadSign.h src/ShapeRoadSignDetector.cpp src/IRoadSignDetector.h src/MyOcr.cpp src/MyOcr.h)
target_link_libraries(main
        ${OpenCV_LIBS}
        Tesseract::libtesseract
        )

add_executable(colorPicker tools/HsvColorPicker.cpp)
target_link_libraries(colorPicker
        ${OpenCV_LIBS}
        )

add_executable(edges tools/Edges.cpp)
target_link_libraries(edges
        ${OpenCV_LIBS}
        )

add_executable(desc tools/Desc.cpp)
target_link_libraries(desc
        ${OpenCV_LIBS}
        )

add_executable(mask_red tools/MaskRed.cpp)
target_link_libraries(mask_red
        ${OpenCV_LIBS}
        )

add_executable(threshold_preview tools/Thresh.cpp)
target_link_libraries(threshold_preview
        ${OpenCV_LIBS}
        )

add_executable(mean_shift tools/MeanShift.cpp)
target_link_libraries(mean_shift
        ${OpenCV_LIBS}
        )

FILE(COPY resources/video DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY resources/imgs DESTINATION ${CMAKE_BINARY_DIR})
