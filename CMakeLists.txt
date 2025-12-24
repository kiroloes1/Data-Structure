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
        GUI/mainwindow.h
        GUI/mainwindow.cpp
        Logic/Interfaces/XmlOperation.h
        Logic/Services/Validation/XmlValidator.cpp
        Logic/Services/Validation/XmlValidator.h
        Logic/Services/Formatting/XmlFormatter.h
        Logic/Services/Formatting/XmlFormatter.cpp
        Logic/JSON/JsonConverter.h
        Logic/JSON/JsonConverter.cpp
        Logic/Services/Minification/XmlMinifier.h
        Logic/Services/Minification/XmlMinifier.cpp
        Logic/Services/Compression/CompressionService.h
        Logic/Services/Compression/CompressionService.cpp
        Logic/Services/Compression/decompression.cpp
        Logic/Services/Graph/GraphVisualizer.h
        Logic/Services/Graph/GraphVisualizer.cpp
        Logic/XmlEditorManager.h
        CLI/CommandLine.h
        CLI/CommandLine.cpp
        
        Logic/Graph/graphParser.cpp
        Logic/Analysis/mostInfluencerId.cpp
        Logic/Analysis/most_active.cpp
        Logic/Analysis/mutualFollowers.cpp
        Logic/Analysis/SuggestionEngine.cpp
        Logic/Analysis/TopicSearch.cpp

        Logic/Tree/XmlTree.cpp
        Logic/Tree/XmlTree.h
        Logic/Tree/XmlTree.h
)

# Rename the output executable to xml_verify
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "xml_verify")

# Link Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE
        GUI
        Logic
        Logic/Services
        Logic/Services/Graph
        Logic/Graph
        Logic/Analysis
        CLI
)
