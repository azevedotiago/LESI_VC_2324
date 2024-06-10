#include "video_processor.h"
#include <opencv2/opencv.hpp>
#include "image_processing.h"
#include "resistor_detection.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <opencv2/tracking.hpp>

// Definição da estrutura do Tracker
struct TrackerInfo {
    cv::Ptr<cv::Tracker> tracker;
    cv::Rect2d boundingBox;
    int value;
};

VideoInfo getVideoInfo(cv::VideoCapture& cap) {
    VideoInfo info{};
    info.totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    info.frameRate = std::round(cap.get(cv::CAP_PROP_FPS));
    info.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    info.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return info;
}

void displayVideoInfo(const VideoInfo& info) {
    std::cout << "Total Frames: " << info.totalFrames << std::endl;
    std::cout << "Frame rate: " << info.frameRate << " FPS" << std::endl;
    std::cout << "Resolucao: " << info.width << "x" << info.height << " pixels" << std::endl;
}

std::vector<cv::Rect> nonMaximumSuppression(const std::vector<cv::Rect>& boxes, float threshold) {
    std::vector<cv::Rect> finalBoxes;
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, std::vector<float>(boxes.size(), 1.0), 0.0, threshold, indices);

    for (int idx : indices) {
        finalBoxes.push_back(boxes[idx]);
    }

    return finalBoxes;
}

void processVideo(cv::VideoCapture& cap) {
    // Configurar o VideoWriter
    VideoInfo info = getVideoInfo(cap);
    std::string outputPath = "../data/samples/output_video.mp4";
    cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), info.frameRate, cv::Size(info.width, info.height));

    if (!writer.isOpened()) {
        std::cerr << "Erro ao abrir o arquivo de saída de vídeo." << std::endl;
        return;
    }

    int framesRead = 0;

    // Lista para dar Tracking às resistências detectadas
    std::vector<TrackerInfo> trackers;

    cv::Mat frame;
    while (cap.read(frame)) {
        framesRead++;

        // Cria o texto com a informação do vídeo
        std::string infoText = "Frames lidos: " + std::to_string(framesRead) +
                               " | " + std::to_string(static_cast<int>(info.frameRate)) + " FPS" +
                               " | " + std::to_string(info.width) + "x" + std::to_string(info.height) + " pixels";

        // Atualizar Trackers existentes
        for (auto it = trackers.begin(); it != trackers.end();) {
            cv::Rect boundingBox = it->boundingBox; // Converte para cv::Rect
            if (it->tracker->update(frame, boundingBox)) {
                it->boundingBox = boundingBox; // Atualiza boundingBox
                cv::rectangle(frame, it->boundingBox, cv::Scalar(0, 255, 0), 2);

                cv::Point2f centroid = calculateCentroid(it->boundingBox);
                cv::circle(frame, centroid, 5, cv::Scalar(255, 0, 0), -1);

                std::string valueText = std::to_string(it->value) + " Ohm";
                cv::putText(frame, valueText, centroid, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
                ++it;
            } else {
                it = trackers.erase(it); // Remove o Tracker se o objeto saiu do frame
            }
        }

        // Detectar novas resistências a cada 30 Frames
        if (framesRead % 30 == 0) {
            // Melhoramento de imagem
            cv::Mat enhancedFrame = enhanceImage(frame);

            // Segmentação de imagem
            cv::Mat segmentedFrame = segmentImage(frame);

            // Deteção de resistências
            std::vector<cv::Rect> resistors = detectResistors(segmentedFrame);

            // Aplicar supressão de não-máximos
            std::vector<cv::Rect> filteredResistors = nonMaximumSuppression(resistors, 0.3);

            // Classificação de resistências válidas
            std::vector<cv::Rect> validResistors;
            std::vector<int> values = classifyResistors(filteredResistors, frame, validResistors);

            // Adicionar novos Trackers para resistências detetadas
            for (size_t i = 0; i < validResistors.size(); ++i) {
                bool alreadyTracked = false;
                for (const auto& trackerInfo : trackers) {
                    double iou = (cv::Rect2d(validResistors[i]) & trackerInfo.boundingBox).area() /
                                 (cv::Rect2d(validResistors[i]) | trackerInfo.boundingBox).area();
                    if (iou > 0.5) {
                        alreadyTracked = true;
                        break;
                    }
                }
                if (!alreadyTracked) {
                    TrackerInfo trackerInfo;
                    trackerInfo.tracker = cv::TrackerCSRT::create();
                    trackerInfo.boundingBox = cv::Rect2d(validResistors[i]);
                    trackerInfo.value = values[i];
                    trackerInfo.tracker->init(frame, trackerInfo.boundingBox);
                    trackers.push_back(trackerInfo);

                    // Imprimir a resistências detetada na consola
                    std::cout << "Frame " << framesRead << ": Resistencia detetada com valor " << values[i] << " Ohm" << std::endl;
                }
            }
        }

        // Desenhar o texto da informação no centro ao fundo do vídeo
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(infoText, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseline);
        cv::Point textOrg((frame.cols - textSize.width) / 2, frame.rows - 10);

        // Altera a escala para 0.7 para negrito e a cor para azul (BGR: 255, 0, 0)
        cv::putText(frame, infoText, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

        // Escrever o frame processado no ficheiro de vídeo
        writer.write(frame);

        // Exibir o frame processado
        cv::imshow("Video", frame);
        if (cv::waitKey(1) >= 0) break;
    }

    // Libertar o VideoWriter
    writer.release();
}
