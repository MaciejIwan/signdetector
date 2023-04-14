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

add_executable(main src/main.cpp)
target_link_libraries(main
        ${OpenCV_LIBS}
        Tesseract::libtesseract
        )

add_executable(colorPicker demo/HsvColorPicker.cpp)
target_link_libraries(colorPicker
        ${OpenCV_LIBS}
        )

add_executable(edges demo/Edges.cpp)
target_link_libraries(edges
        ${OpenCV_LIBS}
        )

FILE(COPY video DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY imgs DESTINATION ${CMAKE_BINARY_DIR})
