//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "resistor_detection.h"
#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Rect> detectResistors(const cv::Mat& frame) {
    std::vector<cv::Rect> resistors;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat hierarchy;

    // Encontra contornos na imagem segmentada
    cv::findContours(frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Cria bounding boxes ao redor dos contornos detectados
    for (const auto& contour : contours) {
        // Filtra contornos pequenos que podem ser ruído
        if (cv::contourArea(contour) > 100) {  // Ajuste este valor conforme necessário
            cv::Rect boundingBox = cv::boundingRect(contour);
            resistors.push_back(boundingBox);
        }
    }

    return resistors;
}

std::vector<int> classifyResistors(const std::vector<cv::Rect>& resistors, const cv::Mat& frame) {
    std::vector<int> values;
    for (const auto& rect : resistors) {
        // Extrair a região da resistência
        cv::Mat resistorROI = frame(rect);

        // Identificar as faixas de cor e calcular o valor
        // Aqui poderias usar algum método como o KMeans ou outra técnica para segmentar as faixas de cor

        int value = 0;  // Calcular o valor baseado nas faixas de cor
        values.push_back(value);
    }

    return values;
}

cv::Point2f calculateCentroid(const cv::Rect& boundingBox) {
    return cv::Point2f(boundingBox.x + boundingBox.width / 2.0, boundingBox.y + boundingBox.height / 2.0);
}
