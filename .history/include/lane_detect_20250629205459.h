#ifndef LANE_DETECT_H
#define LANE_DETECT_H

#include <opencv2/opencv.hpp>
#include <vector>

bool detect_lanes(const cv::Mat &edge, cv::Mat &output, std::vector<cv::Vec4i> &lane_lines);

#endif // LANE_DETECT_H
