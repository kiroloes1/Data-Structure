#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <string>
#include <map>
#include <vector>
#include "graph.h"

// ------------------------ Helper Functions ------------------------

// Removes leading and trailing whitespace
std::string trim(const std::string& str);

// Reads entire XML file into a string
std::string readFile(const std::string& filename);

// Extracts the content of a given XML tag starting from position `pos`
std::string extractTag(const std::string& xml,
                       const std::string& tag,
                       int& pos);

// ------------------------ XML to Graph Loader ------------------------

// Loads users, followers, names, and posts from XML into the graph
void loadXMLtoGraph(const std::string& filename,
                    Graph& g,
                    std::map<int, std::string>& idToName,
                    std::map<int, std::vector<Post>>& idToPosts);

#endif // GRAPH_PARSER_H
