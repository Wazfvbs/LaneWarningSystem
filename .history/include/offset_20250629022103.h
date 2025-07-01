#ifndef offset_H
#define offset_H

// offset.h - Module interface
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

enum OffsetDirection
{
    LEFT,
    CENTER,
    RIGHT
};

struct OffsetResult
{
    int offset_pixels;         // 偏移像素值（正：右偏，负：左偏）
    OffsetDirection direction; // 枚举方向
};

// 输入车道线与图像，输出偏移结构 + 可视化图像
OffsetResult compute_offset_and_draw(cv::Mat &image, const std::vector<cv::Vec4i> &lines);

#endif // offset_H
