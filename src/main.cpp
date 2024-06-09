//
// Created by Tiago Azevedo on 09/06/2024.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "video_processor.h"

int main(int argc, char** argv) {
    // Carrega o vídeo
    std::string videoPath = "../data/video_resistors.mp4";
    cv::VideoCapture cap(videoPath);

    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir o vídeo." << std::endl;
        return -1;
    }

    // Processa o vídeo
    processVideo(cap);

    return 0;
}
