//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "image_processing.h"
#include <opencv2/opencv.hpp>

cv::Mat enhanceImage(const cv::Mat& frame) {
    cv::Mat gray, blurred, edges;

    // Converte para escala de cinza
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Suaviza a imagem para remoção de ruído
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

    // Detecta bordas usando Canny
    cv::Canny(blurred, edges, 50, 150);

    return edges;
}

cv::Mat segmentImage(const cv::Mat& frame) {
    cv::Mat hsv, mask;

    // Converte a imagem para o espaço de cor HSV
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Define os intervalos de cor para a segmentação
    cv::Scalar lowerBound(0, 50, 50);  // Ajusta estes valores conforme necessário
    cv::Scalar upperBound(30, 255, 255);  // Ajusta estes valores conforme necessário

    // Cria a máscara para segmentação
    cv::inRange(hsv, lowerBound, upperBound, mask);

    return mask;
}
