#ifndef OFFSET_H
#define OFFSET_H

#include <opencv2/opencv.hpp>
#include <vector>

enum Direction
{
    LEFT,
    RIGHT,
    CENTER
};

struct OffsetResult
{
    int offset_pixels;
    Direction direction;
};

OffsetResult compute_offset_and_draw(cv::Mat &frame, const std::vector<cv::Vec4i> &lines);

#endif // OFFSET_H