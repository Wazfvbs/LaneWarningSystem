#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera.h"
#include "edge_detect.h"

int main()
{
    cv::Mat frame;
    if (!init_camera())
    {
        std::cerr << "摄像头初始化失败" << std::endl;
        return -1;
    }

    if (!capture_frame(frame))
    {
        std::cerr << "图像采集失败" << std::endl;
        return -1;
    }

    cv::Mat edge;
    sobel_edge_detect(frame, edge);

    // 保存结果图像
    cv::imwrite("results/edge_output.jpg", edge);
    std::cout << "边缘图像已保存至 results/edge_output.jpg" << std::endl;

    return 0;
}
sudo nmcli dev wifi connect 2C:B2:1A:F6:E3:C6 password yzlFT.0528