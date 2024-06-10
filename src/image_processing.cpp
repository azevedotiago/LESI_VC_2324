#include "image_processing.h"
#include <opencv2/opencv.hpp>

cv::Mat enhanceImage(const cv::Mat& frame) {
    cv::Mat gray, blurred, edges;

    // Converte para escala de cinza
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Suaviza a imagem para remoção de ruído
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

    // Deteta bordas usando Canny
    cv::Canny(blurred, edges, 50, 150);

    return edges;
}

cv::Mat segmentImage(const cv::Mat& frame) {
    cv::Mat hsv, mask;

    // Converter a imagem para o espaço de cores HSV
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Definir intervalo de cor para tom castanho
    cv::Scalar lowerBrown(10, 100, 20);
    cv::Scalar upperBrown(30, 255, 200);

    // Aplicar a máscara de cor
    cv::inRange(hsv, lowerBrown, upperBrown, mask);

    // Aplicar operações morfológicas para remover ruídos
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    // Dilatação e Erosão
    cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 2);
    cv::erode(mask, mask, kernel, cv::Point(-1, -1), 2);

    return mask;
}
