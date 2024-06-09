//
// Created by Tiago Azevedo on 09/06/2024.
//

#ifndef RESISTOR_DETECTION_H
#define RESISTOR_DETECTION_H

#include <opencv2/opencv.hpp>

std::vector<cv::Rect> detectResistors(const cv::Mat& frame);

#endif // RESISTOR_DETECTION_H
