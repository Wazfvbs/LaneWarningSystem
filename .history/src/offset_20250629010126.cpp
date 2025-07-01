// offset.c - Module implementation
#include "offset.h"
#include <numeric>

OffsetResult compute_offset_and_draw(cv::Mat &image, const std::vector<cv::Vec4i> &lines)
{
    int img_center = image.cols / 2;
    std::vector<int> left_x, right_x;

    int height = image.rows;
    int region_y = height * 2 / 3; // 仅下方区域有效

    for (const auto &line : lines)
    {
        int x1 = line[0], y1 = line[1];
        int x2 = line[2], y2 = line[3];

        if (y1 < region_y && y2 < region_y)
            continue; // 上方线段忽略
        if (x1 == x2)
            continue; // 忽略垂直线段

        double slope = static_cast<double>(y2 - y1) / (x2 - x1);
        int mid_x = (x1 + x2) / 2;

        if (slope < 0)
            left_x.push_back(mid_x);
        else
            right_x.push_back(mid_x);
    }

    int left_avg = left_x.empty() ? 0 : std::accumulate(left_x.begin(), left_x.end(), 0) / left_x.size();
    int right_avg = right_x.empty() ? image.cols : std::accumulate(right_x.begin(), right_x.end(), 0) / right_x.size();
    int lane_center = (left_avg + right_avg) / 2;

    int offset = lane_center - img_center;

    // 可视化：绘制车道中心线、图像中心线
    cv::line(image, cv::Point(lane_center, height), cv::Point(lane_center, height - 100), cv::Scalar(0, 0, 255), 2);
    cv::line(image, cv::Point(img_center, height), cv::Point(img_center, height - 100), cv::Scalar(255, 255, 255), 1);

    // 添加偏移文本
    std::string text = "Offset: " + std::to_string(offset) + " px ";
    OffsetDirection dir;
    if (offset < -30)
    {
        text += "(Left)";
        dir = LEFT;
    }
    else if (offset > 30)
    {
        text += "(Right)";
        dir = RIGHT;
    }
    else
    {
        text += "(Center)";
        dir = CENTER;
    }

    cv::putText(image, text, cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 255), 2);

    return {offset, dir};
}
