#ifndef edge_detect_H
#define edge_detect_H

// edge_detect.h - Module interface
#pragma once
#include <opencv2/opencv.hpp>

void sobel_edge_detect(const cv::Mat &input, cv::Mat &output);

#endif // edge_detect_H
