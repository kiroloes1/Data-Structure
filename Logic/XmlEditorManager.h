#ifndef XMLEDITORMANAGER_H
#define XMLEDITORMANAGER_H

#include "Formatting/XmlFormatter.h"
#include "Services/Compression/CompressionService.h"
#include "Services/Compression/decompression.h"
#include "Services/Graph/GraphVisualizer.h"
#include "Services/Minification/XmlMinifier.h"
#include "Tree/XmlTree.h"
#include "Validation/XmlValidator.h"
#include "JSON/JsonConverter.h"
#include <fstream>
#include <string>

class XmlEditorManager {
private:
  XmlValidator validator;
  XmlTree treeBuilder;
  XmlFormatter formatter;
  GraphVisualizer visualizer;

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
};

#endif