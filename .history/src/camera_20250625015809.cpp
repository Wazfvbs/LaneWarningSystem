// camera.c - Module implementation
#include "camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>

static cv::VideoCapture cap;

bool init_camera()
{
    cap cap1("http://192.168.31.100:8081/video");
    if (!cap.isOpened())
    {
        std::cerr << "无法打开摄像头" << std::endl;
        return false;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);
    return true;
}

bool capture_frame(cv::Mat &output)
{
    return cap.read(output) && !output.empty();
}
