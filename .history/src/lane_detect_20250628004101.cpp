// lane_detect.c - Module implementation
#include "lane_detect.h"

bool detect_lanes(const cv::Mat& edge, cv::Mat& output, std::vector<cv::Vec4i>& lane_lines) {
    if (edge.empty()) return false;

    cv::HoughLinesP(edge, lane_lines, 1, CV_PI/180, 50, 50, 10);
    output = cv::cvtColor(edge, cv::COLOR_GRAY2BGR); // 用于画线展示

    for (const auto& line : lane_lines) {
        cv::line(output, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]),
                 cv::Scalar(0, 255, 0), 2);
    }
    return true;
}
