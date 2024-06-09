cmake_minimum_required(VERSION 3.10)
project(VisaoComputador)

set(CMAKE_CXX_STANDARD 14)

# Encontra a biblioteca OpenCV
find_package(OpenCV REQUIRED)

# Inclui os diretórios do OpenCV e o diretório include do projeto
include_directories(${OpenCV_INCLUDE_DIRS} include)

# Adiciona os ficheiros fonte
add_executable(VisaoComputador
        src/main.cpp
        src/video_processor.cpp
        src/image_processing.cpp
        src/resistor_detection.cpp
        include/video_processor.h
        include/utils.h
        include/resistor_detection.h
        include/image_processing.h
)

# Liga a biblioteca OpenCV ao projeto
target_link_libraries(VisaoComputador ${OpenCV_LIBS})
