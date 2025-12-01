cmake_minimum_required(VERSION 3.16)

# We name the project matching your folder
project(Data-Structure VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- QT CONFIGURATION ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Define your executable and source files
add_executable(${PROJECT_NAME}
        main.cpp
        main.cpp
        GUI/mainwindow.h
        GUI/mainwindow.cpp
        Logic/Interfaces/XmlOperation.h.cpp
        Logic/Services/Validation/XmlValidator.cpp
        Logic/Services/Validation/XmlValidator.h
        Logic/Services/Formatting/XmlFormatter.h
        Logic/Services/Formatting/XmlFormatter.cpp
        Logic/Services/JSON/JsonConverter.h
        Logic/Services/JSON/JsonConverter.cpp
        Logic/Services/Minification/XmlMinifier.h
        Logic/Services/Minification/XmlMinifier.cpp
        Logic/Services/Compression/CompressionService.h
        Logic/Services/Compression/CompressionService.cpp
        Logic/XmlEditorManager.h
        CLI/CommandLine.h
        CLI/CommandLine.cpp
)

# Link Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE
        GUI
        Logic
        Logic/Services
        CLI
)