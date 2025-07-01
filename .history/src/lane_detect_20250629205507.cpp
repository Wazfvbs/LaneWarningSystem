#include "lane_detect.h"
#include <numeric>

bool detect_lanes(const cv::Mat &edge, cv::Mat &output, std::vector<cv::Vec4i> &lane_lines)
{
    if (edge.empty())
        return false;

    // ROI 区域
    int height = edge.rows;
    int width = edge.cols;
    cv::Mat mask = cv::Mat::zeros(edge.size(), edge.type());
    std::vector<cv::Point> roi = {
        {0, height},
        {width / 2 - 60, height * 0.6},
        {width / 2 + 60, height * 0.6},
        {width, height}};
    cv::fillConvexPoly(mask, roi, cv::Scalar(255));

    cv::Mat masked;
    cv::bitwise_and(edge, mask, masked);

    // HoughLinesP
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(masked, lines, 1, CV_PI / 180, 50, 80, 50);

    // 过滤不合理线段
    lane_lines.clear();
    for (auto &line : lines)
    {
        int x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
        double angle = std::atan2(y2 - y1, x2 - x1) * 180.0 / CV_PI;
        if (std::abs(angle) > 20 && std::abs(angle) < 160)
        {
            lane_lines.push_back(line);
        }
    }

    // 彩色绘图输出
    cv::cvtColor(edge, output, cv::COLOR_GRAY2BGR);
    for (const auto &line : lane_lines)
    {
        cv::line(output, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]),
                 cv::Scalar(0, 255, 0), 2);
    }

    return true;
}