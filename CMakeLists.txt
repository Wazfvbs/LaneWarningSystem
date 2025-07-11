cmake_minimum_required(VERSION 3.10)
project(LaneWarningSystem)
set(OpenCV_DIR "F:/opencv/opencv/build")
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

# 添加 include 目录
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(lane_warning_system
    main.cpp
    src/camera.cpp
    src/edge_detect.cpp
    src/fusion.cpp
    src/lane_detect.cpp
    src/offset.cpp
)

add_executable(lane_warning_video
    main_video_test.cpp
    src/edge_detect.cpp
    src/fusion.cpp
    src/lane_detect.cpp
    src/offset.cpp
)

target_link_libraries(lane_warning_system ${OpenCV_LIBS})
target_link_libraries(lane_warning_video ${OpenCV_LIBS})
