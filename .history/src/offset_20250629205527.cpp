#include "offset.h"
#include <numeric>

OffsetResult compute_offset_and_draw(cv::Mat &frame, const std::vector<cv::Vec4i> &lines)
{
    int mid_x = frame.cols / 2;
    std::vector<int> left_x, right_x;

    for (const auto &line : lines)
    {
        int x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
        int avg_x = (x1 + x2) / 2;
        if (avg_x < mid_x)
            left_x.push_back(avg_x);
        else
            right_x.push_back(avg_x);
    }

    if (left_x.empty() || right_x.empty())
    {
        return {0, CENTER};
    }

    int left = std::accumulate(left_x.begin(), left_x.end(), 0) / left_x.size();
    int right = std::accumulate(right_x.begin(), right_x.end(), 0) / right_x.size();
    int lane_center = (left + right) / 2;
    int offset = mid_x - lane_center;

    // 绘制线
    cv::line(frame, {mid_x, frame.rows}, {mid_x, frame.rows - 80}, cv::Scalar(255, 255, 255), 2);
    cv::line(frame, {lane_center, frame.rows}, {lane_center, frame.rows - 80}, cv::Scalar(0, 0, 255), 2);

    std::string dir_str = (offset < -30) ? "Left" : (offset > 30) ? "Right"
                                                                  : "Center";
    std::string text = "Offset: " + std::to_string(offset) + " px (" + dir_str + ")";
    cv::putText(frame, text, {50, 50}, cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0, 255, 255), 2);

    Direction dir = CENTER;
    if (offset < -30)
        dir = LEFT;
    else if (offset > 30)
        dir = RIGHT;

    return {offset, dir};
}
