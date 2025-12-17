#include "GraphVisualizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

GraphVisualizer::GraphVisualizer() {}

GraphVisualizer::~GraphVisualizer() {}

std::string GraphVisualizer::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }
    std::string xml((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    return xml;
}

std::string GraphVisualizer::extractTag(const std::string& xml, const std::string& tag, int& pos) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";

    size_t start = xml.find(openTag, pos);
    if (start == std::string::npos) return "";

    start += openTag.length();
    size_t end = xml.find(closeTag, start);
    if (end == std::string::npos) return "";

    pos = end + closeTag.length();
    return xml.substr(start, end - start);
}

void GraphVisualizer::parseXML(const std::string& filename) {
    std::string xml = readFile(filename);
    if (xml.empty()) return;

    int pos = 0;
    while (true) {
        size_t userStart = xml.find("<user>", pos);
        if (userStart == std::string::npos) break;

        pos = userStart + 6;

        int temp = pos;
        std::string idStr = extractTag(xml, "id", temp);
        if (idStr.empty()) continue;
        int userId = std::stoi(idStr);

        temp = pos;
        std::string userName = extractTag(xml, "name", temp);

        Node node;
        node.id = userId;
        node.name = userName.empty() ? ("User" + std::to_string(userId)) : userName;

        size_t followersStart = xml.find("<followers>", pos);
        size_t followersEnd = xml.find("</followers>", pos);

        if (followersStart != std::string::npos && followersEnd != std::string::npos) {
            int fPos = followersStart;

            while (true) {
                size_t followerStart = xml.find("<follower>", fPos);
                if (followerStart == std::string::npos || followerStart > followersEnd)
                    break;

                fPos = followerStart + 10;

                int temp2 = fPos;
                std::string followerIdStr = extractTag(xml, "id", temp2);
                if (!followerIdStr.empty()) {
                    int followerId = std::stoi(followerIdStr);
                    node.followers.push_back(followerId);
                }
            }
        }

        nodes[userId] = node;
        size_t userEnd = xml.find("</user>", pos);
        if (userEnd != std::string::npos) {
            pos = userEnd + 7;
        }
    }
}

std::string GraphVisualizer::generateDotFormat() {
    std::ostringstream dot;
    
    dot << "digraph SocialNetwork {\n";
    dot << "    rankdir=LR;\n";
    dot << "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n";
    dot << "    edge [color=gray];\n\n";

    // Add all nodes
    for (const auto& pair : nodes) {
        dot << "    " << pair.first << " [label=\"" << pair.second.name << "\\n(ID:" << pair.first << ")\"];\n";
    }

    dot << "\n";

    // Add all edges (follower -> user)
    for (const auto& pair : nodes) {
        int userId = pair.first;
        for (int followerId : pair.second.followers) {
            dot << "    " << followerId << " -> " << userId << ";\n";
        }
    }

    dot << "}\n";
    return dot.str();
}

std::string GraphVisualizer::generateSVG() {
    std::ostringstream svg;
    
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    
    // Simple circular layout
    int numNodes = nodes.size();
    double centerX = 400, centerY = 300, radius = 200;
    int i = 0;
    
    std::map<int, std::pair<double, double>> positions;
    
    for (const auto& pair : nodes) {
        double angle = 2 * 3.14159 * i / numNodes;
        double x = centerX + radius * std::cos(angle);
        double y = centerY + radius * std::sin(angle);
        positions[pair.first] = {x, y};
        i++;
    }
    
    // Draw edges first
    svg << "  <g id=\"edges\">\n";
    for (const auto& pair : nodes) {
        int userId = pair.first;
        auto userPos = positions[userId];
        for (int followerId : pair.second.followers) {
            if (positions.find(followerId) != positions.end()) {
                auto followerPos = positions[followerId];
                svg << "    <line x1=\"" << followerPos.first << "\" y1=\"" << followerPos.second 
                    << "\" x2=\"" << userPos.first << "\" y2=\"" << userPos.second 
                    << "\" stroke=\"gray\" stroke-width=\"2\" marker-end=\"url(#arrowhead)\"/>\n";
            }
        }
    }
    svg << "  </g>\n";
    
    // Arrow marker
    svg << "  <defs>\n";
    svg << "    <marker id=\"arrowhead\" markerWidth=\"10\" markerHeight=\"10\" refX=\"9\" refY=\"3\" orient=\"auto\">\n";
    svg << "      <polygon points=\"0 0, 10 3, 0 6\" fill=\"gray\"/>\n";
    svg << "    </marker>\n";
    svg << "  </defs>\n";
    
    // Draw nodes
    svg << "  <g id=\"nodes\">\n";
    for (const auto& pair : nodes) {
        auto pos = positions[pair.first];
        svg << "    <circle cx=\"" << pos.first << "\" cy=\"" << pos.second 
            << "\" r=\"30\" fill=\"lightblue\" stroke=\"darkblue\" stroke-width=\"2\"/>\n";
        svg << "    <text x=\"" << pos.first << "\" y=\"" << pos.second 
            << "\" text-anchor=\"middle\" dominant-baseline=\"middle\" font-size=\"12\">" 
            << pair.first << "</text>\n";
        svg << "    <text x=\"" << pos.first << "\" y=\"" << pos.second + 50 
            << "\" text-anchor=\"middle\" font-size=\"10\" fill=\"black\">" 
            << pair.second.name << "</text>\n";
    }
    svg << "  </g>\n";
    
    svg << "</svg>\n";
    return svg.str();
}

bool GraphVisualizer::convertDotToJpg(const std::string& dotContent, const std::string& outputFile) {
    // Save DOT content to temporary file
    std::string dotFile = "temp_graph.dot";
    std::ofstream out(dotFile);
    if (!out.is_open()) {
        std::cerr << "Error: Could not create temporary DOT file" << std::endl;
        return false;
    }
    out << dotContent;
    out.close();

    // Try to use Graphviz's dot command
    std::string command = "dot -Tjpg " + dotFile + " -o " + outputFile;
    int result = system(command.c_str());
    
    // Clean up temp file
    remove(dotFile.c_str());
    
    if (result == 0) {
        std::cout << "Graph successfully generated: " << outputFile << std::endl;
        return true;
    } else {
        std::cerr << "Warning: Graphviz 'dot' command not found or failed." << std::endl;
        std::cerr << "Falling back to SVG output..." << std::endl;
        
        // Fallback: Generate SVG
        std::string svgFile = outputFile.substr(0, outputFile.find_last_of('.')) + ".svg";
        std::ofstream svgOut(svgFile);
        if (!svgOut.is_open()) {
            std::cerr << "Error: Could not create SVG file" << std::endl;
            return false;
        }
        svgOut << generateSVG();
        svgOut.close();
        
        std::cout << "SVG graph generated: " << svgFile << std::endl;
        std::cout << "Note: Install Graphviz for JPG output (https://graphviz.org/download/)" << std::endl;
        return true;
    }
}

bool GraphVisualizer::drawGraph(const std::string& inputXmlFile, const std::string& outputJpgFile) {
    // Clear previous data
    nodes.clear();
    
    // Parse XML
    parseXML(inputXmlFile);
    
    if (nodes.empty()) {
        std::cerr << "Error: No nodes found in XML file" << std::endl;
        return false;
    }
    
    std::cout << "Parsed " << nodes.size() << " users from XML" << std::endl;
    
    // Generate DOT format
    std::string dotContent = generateDotFormat();
    
    // Convert to JPG (or SVG fallback)
    return convertDotToJpg(dotContent, outputJpgFile);
}