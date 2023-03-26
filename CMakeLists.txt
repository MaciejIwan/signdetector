cmake_minimum_required(VERSION 3.24)
project(sign_edges_finder)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4.0 REQUIRED)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
)


add_executable(sign_edges_finder main.cpp)

target_link_libraries(sign_edges_finder
        ${OpenCV_LIBS}
        )