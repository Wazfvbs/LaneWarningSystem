#include "edge_detect.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"
#include "offset.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>

int main()
{
    std::string video_path = "../data/video_challenge.mp4"; // 视频路径
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频文件: " << video_path << std::endl;
        return -1;
    }

    // 创建输出文件夹
    std::filesystem::create_directories("../output");

    FrameFusion fusion;
    cv::Mat frame, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;

    // 显示窗口
    cv::namedWindow("原图", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("边缘图", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("车道偏移检测", cv::WINDOW_AUTOSIZE);

    int frame_id = 0;
    int saved_frame_id = 0;
    auto start_time = std::chrono::steady_clock::now();

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

        // 车道线检测与偏移判断
        if (detect_lanes(edge, lane, lanes))
        {
            OffsetResult result = compute_offset_and_draw(lane, lanes);

            // 偏移打印
            std::string dir_str = (result.direction == LEFT) ? "左偏" : (result.direction == RIGHT) ? "右偏"
                                                                                                      : "居中";
            std::cout << "[偏移] " << result.offset_pixels << " px - " << dir_str << std::endl;

            // 可视化显示
            cv::imshow("原图", frame);
            cv::imshow("边缘图", edge);
            cv::imshow("车道偏移检测", lane);

            // 每隔30帧保存一张偏移检测图像
            if (frame_id % 30 == 0)
            {
                std::string save_path = "../output/frame_" + std::to_string(saved_frame_id++) + ".jpg";
                cv::imwrite(save_path, lane);
            }
        }

        // FPS统计（每10帧）
        if (frame_id % 10 == 0)
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
            float fps = 10000.0f / elapsed;
            std::cout << "[FPS] " << fps << std::endl;
            start_time = now;
        }

        frame_id++;

        // 按 'q' 退出
        if (cv::waitKey(1) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    std::cout << "视频播放完成，已保存图像至 ../output，按任意键退出..." << std::endl;
    cv::waitKey(0);

    return 0;
}
