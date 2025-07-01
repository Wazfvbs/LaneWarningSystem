#include "camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>


static cv::VideoCapture cap;

bool init_camera(const std::string &url)
{
    if (!url.empty())
    {
        cap.open(url); // 支持IP摄像头
    }
    else
    {
        cap.open(0); // 本地摄像头 /dev/video0
    }

    if (!cap.isOpened())
    {
        std::cerr <<"无法打开摄像头或视频流" << std::endl;
        return false;
    }

    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);

    return true;
}

bool capture_frame(cv::Mat &output)
{
    try
    {
        cap >> output;
        return !output.empty();
    }
    catch (...)
    {
        std::cerr << "解码异常，跳过帧" << std::endl;
        return false;
    }
}
