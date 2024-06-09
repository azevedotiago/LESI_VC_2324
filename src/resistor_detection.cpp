//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "resistor_detection.h"
#include <opencv2/opencv.hpp>

std::vector<cv::Rect> detectResistors(const cv::Mat& frame) {
    std::vector<cv::Rect> resistors;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat hierarchy;

    // Encontra contornos na imagem segmentada
    cv::findContours(frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Cria bounding boxes ao redor dos contornos detectados
    for (const auto& contour : contours) {
        cv::Rect boundingBox = cv::boundingRect(contour);
        resistors.push_back(boundingBox);
    }

    return resistors;
}
