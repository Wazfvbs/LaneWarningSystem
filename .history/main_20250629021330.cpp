#include "camera.h"
#include "edge_detect.h"
#include "fusion.h"
#include "lane_detect.h"
#include "offset.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::string ip_stream = "http://admin:admin@172.20.10.4:8081/";
    if (!init_camera(ip_stream))
        return -1;

    cv::Mat frame, fused, edge, lane;
    std::vector<cv::Vec4i> lanes;
    FrameFusion fusion;

    while (true)
    {
        if (capture_frame(frame))
        {
            // 展示原始摄像头图像
            cv::imsh
            cv::imshow("摄像头画面", frame);

            // ① 融合帧
            fusion.addFrame(frame);
            if (!fusion.getFusedFrame(fused))
                continue;

            // ② Sobel 边缘提取
            sobel_edge_detect(fused, edge);

            // ③ 车道线检测
            if (detect_lanes(edge, lane, lanes))
            {
                // 保存车道检测结果图
                cv::imwrite("results/lane_detect.jpg", lane);
                std::cout << "结果保存至 results/lane_detect.jpg" << std::endl;

                // 可视化显示（调试用）
                cv::imshow("边缘图", edge);
                cv::imshow("车道线", lane);
            }
        }
        OffsetResult result = compute_offset_and_draw(lane, lanes);

        // 输出偏移信息
        std::cout << "偏移量：" << result.offset_pixels << " px, ";
        if (result.direction == LEFT)
            std::cout << "左偏" << std::endl;
        else if (result.direction == RIGHT)
            std::cout << "右偏" << std::endl;
        else
            std::cout << "居中" << std::endl;

        // 按下 q 键退出
        if (cv::waitKey(30) == 'q')
            break;
    }

    return 0;
}
