//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "video_processor.h"
#include <opencv2/opencv.hpp>
#include "image_processing.h"
#include "resistor_detection.h"
#include <iostream>
#include <string>
#include <cmath>

VideoInfo getVideoInfo(cv::VideoCapture& cap) {
    VideoInfo info{};
    info.totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    info.frameRate = std::round(cap.get(cv::CAP_PROP_FPS));
    info.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    info.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return info;
}

void displayVideoInfo(const VideoInfo& info) {
    // Exibir as informações do vídeo
    std::cout << "Frames: " << info.totalFrames << std::endl;
    std::cout << "Frame rate: " << info.frameRate << " FPS" << std::endl;
    std::cout << "Resolucao: " << info.width << "x" << info.height << " pixels" << std::endl;
}

void processVideo(cv::VideoCapture& cap) {
    // Obter as informações do vídeo
    VideoInfo info = getVideoInfo(cap);

    // Variável para contar o número de frames lidos
    int framesRead = 0;

    cv::Mat frame;
    while (cap.read(frame)) {
        // Incrementar o contador de frames lidos
        framesRead++;

        // Cria o texto com a informação do vídeo
        std::string infoText = "Frames lidos: " + std::to_string(framesRead) +
                               " | " + std::to_string(static_cast<int>(info.frameRate)) + " FPS" +
                               " | " + std::to_string(info.width) + "x" + std::to_string(info.height) + " pixels";

        // Melhoramento de imagem
        cv::Mat enhancedFrame = enhanceImage(frame);

        // Segmentação de imagem
        cv::Mat segmentedFrame = segmentImage(frame);

        // Deteção de resistências
        std::vector<cv::Rect> resistors = detectResistors(segmentedFrame);

        // Classificação de resistências
        std::vector<int> values = classifyResistors(resistors, frame);

        // Desenhar bounding boxes, centróides e valores
        for (size_t i = 0; i < resistors.size(); ++i) {
            cv::rectangle(frame, resistors[i], cv::Scalar(0, 255, 0), 2);

            cv::Point2f centroid = calculateCentroid(resistors[i]);
            cv::circle(frame, centroid, 5, cv::Scalar(255, 0, 0), -1);

            std::string valueText = std::to_string(values[i]) + " ohms";
            cv::putText(frame, valueText, centroid, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
        }

        // Desenhar o texto da informação no centro ao fundo do vídeo
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(infoText, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseline);
        cv::Point textOrg((frame.cols - textSize.width) / 2, frame.rows - 10);

        // Altera a escala para 1.0 para negrito e a cor para azul (BGR: 255, 0, 0)
        cv::putText(frame, infoText, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

        // Exibir o frame processado
        cv::imshow("Video", frame);
        if (cv::waitKey(30) >= 0) break;
    }
}
