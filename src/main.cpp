#include <iostream>
#include <opencv2/opencv.hpp>
#include "video_processor.h"
#include "image_processing.h"
#include "resistor_detection.h"

int main(int argc, char** argv) {
    std::string videoPath = "../data/video_resistors.mp4";
    cv::VideoCapture cap(videoPath);

    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir o vídeo." << std::endl;
        return -1;
    }

    // Obter e exibir informações do vídeo
    VideoInfo info = getVideoInfo(cap);
    displayVideoInfo(info);

    processVideo(cap);

    return 0;
}
