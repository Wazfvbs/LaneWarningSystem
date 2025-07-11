// edge_detect.c - Module implementation
#include "edge_detect.h"
#include <opencv2/opencv.hpp>

void sobel_edge_detect(const cv::Mat &input, cv::Mat &edge)
{
    if (input.empty())
        return;

    cv::Mat gray;
    if (input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input;

    cv::Mat grad_x, grad_y;
    cv::Mat abs_x, abs_y;

    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3);
    cv::convertScaleAbs(grad_x, abs_x);
    cv::convertScaleAbs(grad_y, abs_y);
    cv::addWeighted(abs_x, 0.5, abs_y, 0.5, 0, edge);
}
