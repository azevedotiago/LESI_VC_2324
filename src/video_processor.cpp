//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "video_processor.h"
#include <opencv2/opencv.hpp>

void processVideo(cv::VideoCapture& cap) {
    cv::Mat frame;
    while (cap.read(frame)) {
        // Exibe o frame
        cv::imshow("Video", frame);
        if (cv::waitKey(30) >= 0) break;
    }
}
