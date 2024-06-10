//
// Created by Tiago Azevedo on 09/06/2024.
//

#ifndef RESISTOR_DETECTION_H
#define RESISTOR_DETECTION_H

#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Rect> detectResistors(const cv::Mat& frame);
std::vector<int> classifyResistors(const std::vector<cv::Rect>& resistors, const cv::Mat& frame, std::vector<cv::Rect>& validResistors);
cv::Point2f calculateCentroid(const cv::Rect& boundingBox);
int identifyResistorValue(const cv::Mat& resistorROI);

#endif // RESISTOR_DETECTION_H
