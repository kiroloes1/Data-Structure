#include "CompressionService.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

static string readFile(const string& path) {
    ifstream in(path);
    if (!in) return "";
    stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

bool isTagNameChar(char c) {
    return isalnum(c) || c == '_' || c == '-' || c == ':';
}

void compressXML(const string& inputFile, const string& outputFile) {
    string content = readFile(inputFile);
    if (content.empty()) {
        cerr << "Error: Input file is empty or cannot be opened." << endl;
        return;
    }


    map<string, string> tagToCode;
    vector<string> codes;
    int counter = 1;


    for (size_t i = 0; i < content.length(); ++i) {
        if (content[i] == '<') {
            size_t j = i + 1;
            while (j < content.length() && content[j] != '>') {
                j++;
            }
            if (j < content.length()) {
                string potentialTag = content.substr(i, j - i + 1);
                
                if (tagToCode.find(potentialTag) == tagToCode.end()) {
                    string code = "@" + to_string(counter++);
                    tagToCode[potentialTag] = code;
                    codes.push_back(potentialTag);
                }
                i = j;
            }
        }
    }

    stringstream compressedBody;
    for (size_t i = 0; i < content.length(); ++i) {
        if (content[i] == '<') {
             size_t j = i + 1;
            while (j < content.length() && content[j] != '>') {
                j++;
            }
            if (j < content.length()) {
                string tag = content.substr(i, j - i + 1);
                if (tagToCode.count(tag)) {
                    compressedBody << tagToCode[tag];
                } else {
                    compressedBody << tag;
                }
                i = j;
            } else {
                compressedBody << content[i];
            }
        } else {
            compressedBody << content[i];
        }
    }

    ofstream out(outputFile);
    if (!out) {
        cerr << "Error: Cannot open output file." << endl;
        return;
    }

    out << tagToCode.size() << endl;

    int currentId = 1;
    for (const string& tag : codes) {
        out << "@" << currentId++ << "=" << tag << endl;
    }

    out << "---" << endl;

    out << compressedBody.str();
    
    out.close();
    cout << "Compression completed. Saved to " << outputFile << endl;
}