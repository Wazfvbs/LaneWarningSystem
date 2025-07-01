#ifndef lane_detect_H
#define lane_detect_H

// lane_detect.h - Module interface
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

bool detect_lanes(const cv::Mat& edge, cv::Mat& output, std::vector<cv::Vec4i>& lanes);


#endif // lane_detect_H
