#include "edge_detect.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>

int main()
{
    std::string video_path = "../../data/2.mp4"; // 视频路径
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频文件: " << video_path << std::endl;
        return -1;
    }

    std::filesystem::create_directories("../../output");

    FrameFusion fusion;
    cv::Mat frame, resized, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;

    //cv::namedWindow("原图", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("车道偏移检测", cv::WINDOW_AUTOSIZE);

    int frame_id = 0;
    int saved_frame_id = 0;
    auto start_time = std::chrono::steady_clock::now();

    const double scale = 0.5;           // 缩放系数
    const int skip_frames = 2;         // 跳帧间隔
    const int fps_stat_interval = 10;  // 统计FPS间隔

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        // 缩放图像加速处理
        cv::resize(frame, resized, cv::Size(), scale, scale);

        fusion.addFrame(resized);
        if (!fusion.getFusedFrame(fused))
        {
            frame_id++;
            continue;
        }

        if (frame_id % skip_frames == 0)
        {
            // Sobel 边缘检测
            sobel_edge_detect(fused, edge);

            // 车道线检测与偏移判断
            if (detect_lanes(edge, lane, lanes))
            {
                OffsetResult result = compute_offset_and_draw(lane, lanes);

                std::string dir_str = (result.direction == LEFT) ? "左偏" : (result.direction == RIGHT) ? "右偏" : "居中";
                std::cout << "[偏移] " << result.offset_pixels << " px - " << dir_str << std::endl;

                // 缩放回原尺寸显示（车道线图）
                cv::Mat lane_resized;
                cv::resize(lane, lane_resized, frame.size());

                //cv::imshow("原图", frame);
                cv::imshow("车道偏移检测", lane_resized);

                if (frame_id % 30 == 0)
                {
                    std::string save_path = "../../output/frame_" + std::to_string(saved_frame_id++) + ".jpg";
                    cv::imwrite(save_path, lane_resized);
                }
            }

            // FPS统计
            if (frame_id % fps_stat_interval == 0)
            {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
                float fps = (1000.0f * fps_stat_interval) / elapsed;
                std::cout << "[FPS] " << fps << std::endl;
                start_time = now;
            }
        }

        frame_id++;

        if (cv::waitKey(1) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    std::cout << "视频播放完成，已保存图像至 ../output，按任意键退出..." << std::endl;
    cv::waitKey(0);

    return 0;
}