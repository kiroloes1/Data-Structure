#ifndef XMLEDITORMANAGER_H
#define XMLEDITORMANAGER_H

#include "Analysis/SuggestionEngine.h"
#include "Analysis/TopicSearch.h"
#include "Analysis/mostInfluencerId.h"
#include "Analysis/most_active.h"
#include "Analysis/mutualFollowers.h"
#include "Formatting/XmlFormatter.h"
#include "Graph/graphParser.h"
#include "Services/Compression/CompressionService.h"
#include "Services/Compression/decompression.h"
#include "Services/Graph/GraphVisualizer.h"
#include "Services/Minification/XmlMinifier.h"
#include "Tree/XmlTree.h"
#include "Validation/XmlValidator.h"
#include "JSON/JsonConverter.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class XmlEditorManager {
private:
  XmlValidator validator;
  XmlTree treeBuilder;
  XmlFormatter formatter;
  GraphVisualizer visualizer;

  // Helper to load graph from string content (via temp file)
  bool loadGraph(const std::string &xmlContent, Graph &g,
                 std::map<int, std::string> &idToName,
                 std::map<int, std::vector<Post>> &idToPosts) {
    // Parser expects a file. Save to temp.
    std::string tempFile = "logic_temp_graph.xml";
    std::ofstream out(tempFile);
    out << xmlContent;
    out.close();

    try {
      loadXMLtoGraph(tempFile, g, idToName, idToPosts);
      return true;
    } catch (...) {
      return false;
    }
  }

public:
  XmlEditorManager() {}

  bool verifyXml(const std::string &xmlContent, std::string &errorLog) {
    return validator.validate(xmlContent, errorLog);
  }

  std::string fixXml(const std::string &xmlContent) {
    return validator.fix(xmlContent);
  }

  std::string formatXml(const std::string &xmlContent) {
    try {
      return formatter.prettify_xml(xmlContent);
    } catch (...) {
      return "";
    }
  }

  std::string convertToJson(const std::string &xmlContent) {
    Node *root = treeBuilder.parseXML(xmlContent);
    if (!root)
      return "";
    std::string json = xmlToJSON(root);
    delete root;
    return json;
  }

  // File-based operations
  void minifyFile(const std::string &input, const std::string &output) {
    minifyXML(input, output);
  }

  void compressFile(const std::string &input, const std::string &output) {
    compressXML(input, output);
  }

  bool decompressFileWrapper(const std::string &input,
                             const std::string &output) {
    return decompressFile(input, output);
  }

  bool drawGraph(const std::string &input, const std::string &output) {
    return visualizer.drawGraph(input, output);
  }

  // Wrappers for Graph Analysis
  std::string getMostInfluencer(const std::string &xmlContent) {
    Graph g;
    std::map<int, std::string> names;
    std::map<int, std::vector<Post>> posts;
    if (!loadGraph(xmlContent, g, names, posts))
      return "Error loading graph.";

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    print_influencer_result(g, names);

    std::cout.rdbuf(old);
    return buffer.str();
  }

  std::string getMostActive(const std::string &xmlContent) {
    Graph g;
    std::map<int, std::string> names;
    std::map<int, std::vector<Post>> posts;
    if (!loadGraph(xmlContent, g, names, posts))
      return "Error loading graph.";

    std::pair<int, std::string> active = most_active_user(g, names);
    return "Most Active User: " + active.second +
           " (ID: " + std::to_string(active.first) + ")";
  }

  std::string getMutualFollowers(const std::string &xmlContent,
                                 const std::vector<int> &ids) {
    Graph g;
    std::map<int, std::string> names;
    std::map<int, std::vector<Post>> posts;
    if (!loadGraph(xmlContent, g, names, posts))
      return "Error loading graph.";

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    print_mutual_followers(g, ids, names);

    std::cout.rdbuf(old);
    return buffer.str();
  }

  std::string getSuggestions(const std::string &xmlContent, int userId) {
    Graph g;
    std::map<int, std::string> names;
    std::map<int, std::vector<Post>> posts;
    if (!loadGraph(xmlContent, g, names, posts))
      return "Error loading graph.";

    std::vector<int> suggestions = suggest_users(userId, g);

    if (suggestions.empty())
      return "No suggestions available.";

    std::string result =
        "Suggestions for User " + std::to_string(userId) + ":\n";
    for (int id : suggestions) {
      std::string name = (names.count(id) ? names[id] : "Unknown");
      result += "- " + name + " (ID: " + std::to_string(id) + ")\n";
    }
    return result;
  }

  std::string searchPost(const std::string &xmlContent,
                         const std::string &query) {
    Graph g;
    std::map<int, std::string> names;
    std::map<int, std::vector<Post>> posts;
    if (!loadGraph(xmlContent, g, names, posts))
      return "Error loading graph.";

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    searchByTopic(query, posts, names);

    std::cout.rdbuf(old);
    return buffer.str();
  }
};

#endif