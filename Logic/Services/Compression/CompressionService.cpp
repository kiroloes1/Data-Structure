#include "CompressionService.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

static string readFile(const string &path) {
  ifstream in(path, ios::binary);
  if (!in)
    return "";
  stringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

// NEW: helper to get file size
static long long getFileSize(const string &path) {
  ifstream in(path, ios::binary | ios::ate);
  if (!in)
    return 0;
  return in.tellg();
}

bool isTagNameChar(char c) {
  return isalnum(c) || c == '_' || c == '-' || c == ':';
}

void compressXML(const string &inputFile, const string &outputFile) {

  // NEW: original file size
  long long originalSize = getFileSize(inputFile);

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

        if (potentialTag.find('\n') == string::npos) {
          if (tagToCode.find(potentialTag) == tagToCode.end()) {
            string code = "@" + to_string(counter++) + ";";
            tagToCode[potentialTag] = code;
            codes.push_back(potentialTag);
          }
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
      if (content[i] == '@') {
        compressedBody << "@@";
      } else {
        compressedBody << content[i];
      }
    }
  }

  ofstream out(outputFile, ios::binary);
  if (!out) {
    cerr << "Error: Cannot open output file." << endl;
    return;
  }

  out << tagToCode.size() << endl;

  int currentId = 1;
  for (const string &tag : codes) {
    out << "@" << currentId++ << ";=" << tag << endl;
  }

  out << "---" << endl;
  out << compressedBody.str();

  out.close();

  //  compressed file size
  long long compressedSize = getFileSize(outputFile);

  // compression percentage
  if (originalSize > 0) {
    double compressionPercent =
        (1.0 - (double)compressedSize / originalSize) * 100.0;

    cout << "Compression completed. Saved to " << outputFile << endl;
    cout << "Original size    : " << originalSize << " bytes" << endl;
    cout << "Compressed size  : " << compressedSize << " bytes" << endl;
    cout << "Compression ratio: " << compressionPercent << " %" << endl;
  } else {
    cout << "Compression completed. Saved to " << outputFile << endl;
  }
}