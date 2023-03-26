cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4.0 REQUIRED)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

project(sign_edges_finder)

add_executable(sign_edges_finder main.cpp)
target_link_libraries(sign_edges_finder
        ${OpenCV_LIBS}
        )

add_executable(desc desc.cpp) # Add new target
target_link_libraries(desc
        ${OpenCV_LIBS}
        )

add_executable(thresh thresh.cpp thresh.cpp) # Add new target
target_link_libraries(thresh
        ${OpenCV_LIBS}
        )

add_executable(maskRed maskRed.cpp)
target_link_libraries(maskRed
        ${OpenCV_LIBS}
        )

add_executable(meanShift meanShift.cpp meanShift.cpp)
target_link_libraries(meanShift
        ${OpenCV_LIBS}
        )

FILE(COPY imgs DESTINATION ${CMAKE_BINARY_DIR})
