#ifndef DATA_STRUCTURE_GRAPHVISUALIZER_H
#define DATA_STRUCTURE_GRAPHVISUALIZER_H

#include <string>
#include <vector>
#include <map>

class GraphVisualizer {
private:
    struct Node {
        int id;
        std::string name;
        std::vector<int> followers;
    };

    std::map<int, Node> nodes;
    
    std::string generateDotFormat();
    bool convertDotToJpg(const std::string& dotContent, const std::string& outputFile);
    std::string readFile(const std::string& filename);
    std::string extractTag(const std::string& xml, const std::string& tag, int& pos);
    void parseXML(const std::string& filename);

public:
    GraphVisualizer();
    ~GraphVisualizer();
    
    // Main function to draw graph from XML to JPG
    bool drawGraph(const std::string& inputXmlFile, const std::string& outputJpgFile);
    
    // Generate SVG string (fallback if no external tools available)
    std::string generateSVG();
};

#endif // DATA_STRUCTURE_GRAPHVISUALIZER_H