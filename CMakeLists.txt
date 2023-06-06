    # ========================================
    # Setup CMake
    # ========================================
    cmake_minimum_required(VERSION 3.16)
    set(CMAKE_CXX_STANDARD 20)

    # ========================================
    # Setup project and dependencies
    # ========================================
    project(SIGN_DETECTOR)

    find_package(OpenCV 4.0 REQUIRED)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET tesseract lept)
    add_library(Tesseract::libtesseract ALIAS PkgConfig::Tesseract)

    set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/qt5/")

    find_package(Qt5 REQUIRED COMPONENTS
            Core
            Gui
            Widgets
            Multimedia
            )

    include_directories(
            ${OpenCV_INCLUDE_DIRS}
            ${Tesseract_INCLUDE_DIRS}
    )

    # ========================================
    # Setup library
    # ========================================
    add_library(RoadSignDetection
            include/App.h
            include/CircularBuffer.h
            include/CircularRoadSignDetector.h
            include/Common.h
            include/FrameProvider.h
            include/IRoadSignDetector.h
            include/NotificationPlayer.h
            include/Ocr.h
            include/models/RoadSign.h
            include/models/SpeedLimitSign.h
            include/SignDrawer.h
            include/ShapeRoadSignDetector.h
            src/App.cpp
            src/CircularRoadSignDetector.cpp
            src/NotificationPlayer.cpp
            src/Ocr.cpp
            src/SignDrawer.cpp
            src/ShapeRoadSignDetector.cpp
            src/Util.cpp
            )
    target_link_libraries(RoadSignDetection
            ${OpenCV_LIBS}
            Qt5::Multimedia
            Qt5::Widgets
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
            Qt5::Multimedia
            Qt5::Widgets
            pthread
            )

    # ========================================
    # Setup tools
    # ========================================
    add_executable(colorPicker
            tools/HsvColorPicker.cpp
            )
    target_link_libraries(colorPicker
            ${OpenCV_LIBS}
            )

    add_executable(edges
            tools/Edges.cpp
            )
    target_link_libraries(edges
            ${OpenCV_LIBS}
            )

    add_executable(desc
            tools/Desc.cpp
            )
    target_link_libraries(desc
            ${OpenCV_LIBS}
            )

    add_executable(mask_red
            tools/MaskRed.cpp
            )
    target_link_libraries(mask_red
            ${OpenCV_LIBS}
            )

    add_executable(threshold_preview
            tools/Threshold.cpp
            )
    target_link_libraries(threshold_preview
            ${OpenCV_LIBS}
            )

    add_executable(mean_shift
            tools/MeanShift.cpp
            )
    target_link_libraries(mean_shift
            ${OpenCV_LIBS}
            )

    add_executable(video_editor
            tools/VideoEditor.cpp
            )
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

    add_executable(tests
            tests/include/utils/VideoTestUtils.h
            tests/OcrTest.cpp
            tests/RoadSignDetectorTest.cpp
            tests/VideoTest.cpp
            tests/utils/VideoTestUtils.cpp
            )
    target_link_libraries(tests
            ${OpenCV_LIBS}
            GTest::gtest_main
            RoadSignDetection
            Tesseract::libtesseract
            )

    enable_testing()
    gtest_discover_tests(tests)

    # ========================================
    # Copy resources to build directory
    # ========================================
    FILE(COPY resources/img DESTINATION ${CMAKE_BINARY_DIR})
    FILE(COPY resources/sound DESTINATION ${CMAKE_BINARY_DIR})
    FILE(COPY resources/video DESTINATION ${CMAKE_BINARY_DIR})
