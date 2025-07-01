#include "edge_detect.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"

#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::string video_path = "../data/video_challenge.mp4"; // 你的视频路径
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频文件: " << video_path << std::endl;
        return -1;
    }

    FrameFusion fusion;
    cv::Mat frame, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;

    // 创建窗口
    cv::namedWindow("原图", cv::WINDOW_NORMAL);
    cv::namedWindow("边缘图", cv::WINDOW_NORMAL);
    cv::namedWindow("车道偏移检测", cv::WINDOW_NORMAL);

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        fusion.addFrame(frame);
        if (!fusion.getFusedFrame(fused))
            continue;

        // Sobel 边缘检测
        sobel_edge_detect(fused, edge);

        // 车道检测
        if (detect_lanes(edge, lane, lanes))
        {
            // 车道偏移判断与绘制
            OffsetResult result = compute_offset_and_draw(lane, lanes);

            // 打印偏移信息
            std::string dir_str = (result.direction == LEFT) ? "← 左偏" : (result.direction == RIGHT) ? "→ 右偏"
                                                                                                      : "✓ 居中";
            std::cout << "[偏移] " << result.offset_pixels << " px - " << dir_str << std::endl;

            // 可视化三个窗口
            cv::imshow("原图", frame);
            cv::imshow("边缘图", edge);
            cv::imshow("车道偏移检测", lane);
        }

        // 按 q 键退出
        if (cv::waitKey(30) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
