#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>

struct VideoInfo {
    int totalFrames;
    double frameRate;
    int width;
    int height;
};

void processVideo(cv::VideoCapture& cap);
void displayVideoInfo(const VideoInfo& info);
VideoInfo getVideoInfo(cv::VideoCapture& cap);

#endif // VIDEO_PROCESSOR_H
