#ifndef EXTRACTTAG_H
#define EXTRACTTAG_H

#include <string>
#include <vector>

using namespace std;

// Trim spaces from a string
string trim(string file);

// Read XML file and return its content as a string
string readXMLFile(const string& filename);

// Convert XML string to vector of tokens (tags and content)
vector<string> convertXMLToVector(const string& xmlString);

#endif
