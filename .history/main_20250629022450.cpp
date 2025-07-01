#include "edge_detect.h"
#include "camera.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"

#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::string ip_stream = "http://admin:admin@172.20.10.4:8081/";

    if (!init_camera(ip_stream))
    {
        std::cerr << "摄像头初始化失败。" << std::endl;
        return -1;
    }

    FrameFusion fusion;
    cv::Mat frame, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;

    cv::namedWindow("摄像头画面", cv::WINDOW_NORMAL);
    cv::namedWindow("边缘图", cv::WINDOW_NORMAL);
    cv::namedWindow("车道线", cv::WINDOW_NORMAL);

    while (true)
    {
        if (!capture_frame(frame))
            continue;

        cv::imshow("摄像头画面", frame);

        // 融合处理
        fusion.addFrame(frame);
        if (!fusion.getFusedFrame(fused))
            continue;

        // Sobel边缘检测
        sobel_edge_detect(fused, edge);

        // 车道线检测
        if (detect_lanes(edge, lane, lanes))
        {
            OffsetResult result = compute_offset_and_draw(lane, lanes);

            std::string dir_str = (result.direction == LEFT)    ? "左偏"
                                  : (result.direction == RIGHT) ? "右偏"
                                                                : "居中";

            std::cout << "[偏移] " << result.offset_pixels << " px - " << dir_str << std::endl;

            // 可视化输出
            cv::imshow("边缘图", edge);
            cv::imshow("车道线", lane);

            // 保存检测结果
            cv::imwrite("results/lane_detect.jpg", lane);
        }

        if (cv::waitKey(30) == 'q')
            break;
    }

    return 0;
}
// End of file: main.cpp
// This code initializes a camera stream, captures frames, applies Sobel edge detection, detects lane