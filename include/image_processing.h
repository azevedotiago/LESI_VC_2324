#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/opencv.hpp>

cv::Mat segmentImage(const cv::Mat& frame);
cv::Mat enhanceImage(const cv::Mat& frame);

#endif // IMAGE_PROCESSING_H
