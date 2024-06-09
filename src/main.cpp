//
// Created by Tiago Azevedo on 09/06/2024.
//

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

    cv::Mat frame;
    while (cap.read(frame)) {
        // Melhoramento de imagem
        cv::Mat enhancedFrame = enhanceImage(frame);

        // Segmentação de imagem
        cv::Mat segmentedFrame = segmentImage(frame);

        // Deteção de resistências
        std::vector<cv::Rect> resistors = detectResistors(segmentedFrame);

        // Desenhar bounding boxes ao redor das resistências
        for (const auto& rect : resistors) {
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
        }

        // Exibir o frame processado
        cv::imshow("Video", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}
