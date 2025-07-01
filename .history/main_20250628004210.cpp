#include "camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // 使用IP摄像头调试
    std::string ip_stream = "http://admin:admin@172.20.10.4:8081/";

    if (!init_camera(ip_stream))
    {
        return -1;
    }

    cv::Mat frame;
    while (true)
    {
        if (capture_frame(frame))
        {
            cv::imshow("摄像头画面", frame);
        }
        if (cv::waitKey(30) == 'q')
            break;
    }

    return 0;
}
