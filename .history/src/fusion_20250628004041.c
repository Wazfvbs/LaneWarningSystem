// fusion.c - Module implementation
#include "fusion.h"

FrameFusion::FrameFusion(int maxFrames) : maxSize(maxFrames) {}

void FrameFusion::addFrame(const cv::Mat& frame) {
    if (frame.empty()) return;
    buffer.push_back(frame.clone());
    if (buffer.size() > maxSize) {
        buffer.pop_front();
    }
}

bool FrameFusion::getFusedFrame(cv::Mat& output) {
    if (buffer.empty()) return false;

    cv::Mat acc = cv::Mat::zeros(buffer.front().size(), CV_32FC3);
    for (const auto& f : buffer) {
        cv::Mat temp;
        f.convertTo(temp, CV_32FC3);
        acc += temp;
    }
    acc /= static_cast<float>(buffer.size());
    acc.convertTo(output, CV_8UC3);
    return true;
}
