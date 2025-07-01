#include "lane_detect.h"

bool detect_lanes(const cv::Mat &edge, cv::Mat &output, std::vector<cv::Vec4i> &lane_lines)
{
    if (edge.empty())
        return false;

    // 车道线检测
    cv::HoughLinesP(edge, lane_lines, 1, CV_PI / 180, 50, 50, 10);

    // 输出边缘图像类型确认

    // 转换为彩色图以供偏移绘图（不画绿色线）
    cv::cvtColor(edge, output, cv::COLOR_GRAY2BGR);

    return true;
}
