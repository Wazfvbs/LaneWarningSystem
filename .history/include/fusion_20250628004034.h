#ifndef fusion_H
#define fusion_H

// fusion.h - Module interface
#pragma once
#include <opencv2/opencv.hpp>
#include <deque>

class FrameFusion {
public:
    FrameFusion(int maxFrames = 3);
    void addFrame(const cv::Mat& frame);
    bool getFusedFrame(cv::Mat& output);
private:
    int maxSize;
    std::deque<cv::Mat> buffer;
};

#endif // fusion_H
