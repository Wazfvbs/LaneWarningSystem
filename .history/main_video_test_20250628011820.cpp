#include "edge_detect.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"

#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::string video_path = "../data/video_challenge.mp4"; // 替换为你的视频
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频文件: " << video_path << std::endl;
        return -1;
    }

    FrameFusion fusion;
    cv::Mat frame, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        fusion.addFrame(frame);
        if (!fusion.getFusedFrame(fused))
            continue;

        sobel_edge_detect(fused, edge);

        if (detect_lanes(edge, lane, lanes))
        {
            OffsetResult result = compute_offset_and_draw(lane, lanes);

            // 输出偏移信息
            std::string dir_str = (result.direction == LEFT) ? "Left" : (result.direction == RIGHT) ? "Right"
                                                                                                    : "Center";
            std::cout << "[偏移] " << result.offset_pixels << " px - " << dir_str << std::endl;

            // 显示带标注图像
            cv::imshow("Lane Detection", lane);
        }

        if (cv::waitKey(30) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
