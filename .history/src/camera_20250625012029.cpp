// camera.c - Module implementation
#include "camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>

static cv::VideoCapture cap;

bool init_camera()
{
    cap.open(0); // 本地 USB 摄像头（/dev/video0）
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
