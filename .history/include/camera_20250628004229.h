#ifndef camera_H
#define camera_H

// camera.h - Module interface
#pragma once
#include <opencv2/opencv.hpp>

bool init_camera();
bool capture_frame(cv::Mat &output);

#endif // camera_H
