# ========================================
# Setup CMake
# ========================================
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)


# ========================================
# Setup project and dependencies
# ========================================
project(SIGN_DETECTOR)

find_package(OpenCV 4.0 REQUIRED)
find_package(Tesseract 4)
if (NOT Tesseract_FOUND)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET tesseract lept)
    add_library(Tesseract::libtesseract ALIAS PkgConfig::Tesseract)
endif ()


set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/qt5/")

find_library(LIBAO_LIBRARY NAMES ao PATHS usr/lib/x86_64-linux-gnu/ REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MPG123 REQUIRED IMPORTED_TARGET libmpg123)

find_package(Qt5 REQUIRED COMPONENTS
        Widgets
        Core
        Gui
        Multimedia
        )


include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
        ${LIBAO_INCLUDE_DIR}
        )


# ========================================
# Setup library
# ========================================
add_library(RoadSignDetection
        include/models/RoadSign.h
        include/IRoadSignDetector.h
        src/ShapeRoadSignDetector.cpp
        include/ShapeRoadSignDetector.h
        include/Ocr.h
        src/Ocr.cpp
        include/models/SpeedLimitSign.h
        include/Common.h
        include/CircularBuffer.h
        src/Util.cpp
        src/CircularRoadSignDetector.cpp
        include/CircularRoadSignDetector.h
        src/NotificationPlayer.cpp
        include/NotificationPlayer.h
        )
target_link_libraries(RoadSignDetection
        Qt5::Widgets
        Qt5::Multimedia
        )


# ========================================
# Setup executable
# ========================================
add_executable(main
        src/Main.cpp
        )
target_link_libraries(main
        ${OpenCV_LIBS}
        RoadSignDetection
        Tesseract::libtesseract
        Qt5::Widgets
        Qt5::Multimedia
        ${LIBAO_LIBRARY}
        PkgConfig::MPG123
        )



# ========================================
# Setup tools
# ========================================
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

add_executable(threshold_preview tools/Threshold.cpp)
target_link_libraries(threshold_preview
        ${OpenCV_LIBS}
        )

add_executable(mean_shift tools/MeanShift.cpp)
target_link_libraries(mean_shift
        ${OpenCV_LIBS}
        )

add_executable(perspective tools/Perspective.cpp)
target_link_libraries(perspective
        ${OpenCV_LIBS}
        )

add_executable(video_editor tools/VideoEditor.cpp)
target_link_libraries(video_editor
        ${OpenCV_LIBS}
        )


# ========================================
# Setup testing
# ========================================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


add_executable(
        tests
        tests/utils/VideoTestUtils.cpp
        tests/OcrTest.cpp src/Util.cpp
        tests/VideoTest.cpp
        tests/RoadSignDetectorTest.cpp
)
target_link_libraries(tests
        ${OpenCV_LIBS}
        Tesseract::libtesseract
        GTest::gtest_main
        RoadSignDetection
        ${LIBAO_LIBRARY}
        PkgConfig::MPG123
        Qt5::Widgets
        Qt5::Multimedia
        )
enable_testing()
gtest_discover_tests(tests)


# ========================================
# Copy resources to build directory
# ========================================
FILE(COPY resources/video DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY resources/img DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY resources/sound DESTINATION ${CMAKE_BINARY_DIR})
