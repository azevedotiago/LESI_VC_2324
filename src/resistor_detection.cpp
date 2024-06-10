//
// Created by Tiago Azevedo on 09/06/2024.
//

#include "resistor_detection.h"
#include <opencv2/opencv.hpp>
#include <vector>

// Define os intervalos de cor para as faixas das resistências
struct ColorRange {
    cv::Scalar lowerBound;
    cv::Scalar upperBound;
    int value;
};

// Definir intervalos de cor em HSV para cada faixa
std::vector<ColorRange> colorRanges = {
    {cv::Scalar(0, 0, 0), cv::Scalar(180, 255, 30), 0},         // Preto
    {cv::Scalar(0, 50, 20), cv::Scalar(20, 255, 200), 1},       // Castanho
    {cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), 2},       // Vermelho
    {cv::Scalar(10, 100, 100), cv::Scalar(25, 255, 255), 3},    // Laranja
    {cv::Scalar(25, 100, 100), cv::Scalar(35, 255, 255), 4},    // Amarelo
    {cv::Scalar(35, 50, 50), cv::Scalar(85, 255, 255), 5},      // Verde
    {cv::Scalar(85, 50, 50), cv::Scalar(125, 255, 255), 6},     // Azul
    {cv::Scalar(125, 50, 50), cv::Scalar(145, 255, 255), 7},    // Violeta
    {cv::Scalar(0, 0, 50), cv::Scalar(180, 50, 200), 8},        // Cinza
    {cv::Scalar(0, 0, 200), cv::Scalar(180, 20, 255), 9}        // Branco
};


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

int identifyResistorValue(const cv::Mat& resistorROI) {
    cv::Mat hsv;
    cv::cvtColor(resistorROI, hsv, cv::COLOR_BGR2HSV);

    std::vector<int> colorBands;
    for (const auto& range : colorRanges) {
        cv::Mat mask;
        cv::inRange(hsv, range.lowerBound, range.upperBound, mask);
        if (cv::countNonZero(mask) > 0) {
            colorBands.push_back(range.value);
        }
    }

    if (colorBands.size() < 4) return -1;  // Menos de 4 faixas não pode ser uma resistência válida

    int value = 0;
    value += colorBands[0] * 10 + colorBands[1];
    value *= std::pow(10, colorBands[2]);

    return value;
}

std::vector<int> classifyResistors(const std::vector<cv::Rect>& resistors, const cv::Mat& frame) {
    std::vector<int> values;
    for (const auto& rect : resistors) {
        cv::Mat resistorROI = frame(rect);

        int value = identifyResistorValue(resistorROI);
        values.push_back(value);
    }

    return values;
}

cv::Point2f calculateCentroid(const cv::Rect& boundingBox) {
    return cv::Point2f(boundingBox.x + boundingBox.width / 2.0, boundingBox.y + boundingBox.height / 2.0);
}
