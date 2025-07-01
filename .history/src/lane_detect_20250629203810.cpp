#include "lane_detect.h"
#include <iostream>

bool detect_lanes(const cv::Mat &edge, cv::Mat &output, std::vector<cv::Vec4i> &lane_lines)
{
    if (edge.empty())
        return false;

    // 获取 ROI（图像下1/3区域）
    int roi_y = edge.rows * 2 / 3;
    cv::Mat roi = edge(cv::Rect(0, roi_y, edge.cols, edge.rows - roi_y));

    // Hough 检测车道线
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(roi, lines, 1, CV_PI / 180, 50, 50, 20);

    // 调整线段 y 偏移（因为是 ROI 中的）
    for (auto &line : lines)
    {
        line[1] += roi_y;
        line[3] += roi_y;
    }

    lane_lines = lines;                            // 原始线段保存下来
    output = cv::Mat::zeros(edge.size(), CV_8UC3); // 创建彩色图

    // 可视化原始线段（调试用）
    for (const auto &line : lines)
    {
        cv::line(output, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]),
                 cv::Scalar(0, 255, 0), 1);
    }

    return true;
}
