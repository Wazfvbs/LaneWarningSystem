#include "camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>

static cv::VideoCapture cap;

bool init_camera()
{
    // 如果你使用 IP 摄像头（HTTP MJPEG 流）
    std::string url = "http://192.168.2.127:8081/video";
    if (!cap.open(url))
    {
        std::cerr << "无法打开 IP 摄像头: " << url << std::endl;
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
