#include "offset.h"
#include <opencv2/opencv.hpp>
#include <numeric>

OffsetResult compute_offset_and_draw(cv::Mat &image, const std::vector<cv::Vec4i> &lines)
{
    int width = image.cols;
    int height = image.rows;
    int center_x = width / 2;

    std::vector<cv::Point> left_points, right_points;

    // 根据斜率与位置筛选线段
    for (const auto &line : lines)
    {
        int x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
        double slope = (double)(y2 - y1) / (x2 - x1 + 1e-6);

        // 过滤近乎水平或垂直线段
        if (fabs(slope) < 0.3 || fabs(slope) > 5.0)
            continue;

        if (slope < 0 && x1 < center_x && x2 < center_x)
        {
            left_points.push_back(cv::Point((x1 + x2) / 2, (y1 + y2) / 2));
        }
        else if (slope > 0 && x1 > center_x && x2 > center_x)
        {
            right_points.push_back(cv::Point((x1 + x2) / 2, (y1 + y2) / 2));
        }
    }

    int left_x = -1, right_x = -1;
    if (!left_points.empty())
        left_x = std::accumulate(left_points.begin(), left_points.end(), 0,
                                 [](int sum, const cv::Point &p)
                                 { return sum + p.x; }) /
                 left_points.size();

    if (!right_points.empty())
        right_x = std::accumulate(right_points.begin(), right_points.end(), 0,
                                  [](int sum, const cv::Point &p)
                                  { return sum + p.x; }) /
                  right_points.size();

    int lane_center = -1;
    if (left_x >= 0 && right_x >= 0)
    {
        lane_center = (left_x + right_x) / 2;
    }
    else if (left_x >= 0)
    {
        lane_center = left_x + 200; // 估计宽度
    }
    else if (right_x >= 0)
    {
        lane_center = right_x - 200;
    }
    else
    {
        lane_center = center_x;
    }

    int offset = lane_center - center_x;

    // 绘制偏移线
    cv::line(image, cv::Point(center_x, height), cv::Point(center_x, height - 100), cv::Scalar(255, 255, 255), 2);
    cv::line(image, cv::Point(lane_center, height), cv::Point(lane_center, height - 100), cv::Scalar(0, 0, 255), 2);

    // 偏移信息文字
    std::string info = "Offset: " + std::to_string(offset) + " px ";
    if (offset < -20)
        info += "(Left)";
    else if (offset > 20)
        info += "(Right)";
    else
        info += "(Center)";
    cv::putText(image, info, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 255, 255), 2);

    OffsetResult result;
    result.offset_pixels = offset;
    if (offset < -20)
        result.direction = LEFT;
    else if (offset > 20)
        result.direction = RIGHT;
    else
        result.direction = CENTER;

    return result;
}
