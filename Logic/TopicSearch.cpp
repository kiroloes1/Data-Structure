#include "TopicSearch.h"
#include "XmlFormatter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


string extractBetween(const string& line,
                      const string& startTag,
                      const string& endTag) {
    size_t start = line.find(startTag);
    size_t end = line.find(endTag);

    if (start == string::npos || end == string::npos)
        return "";

    start += startTag.length();
    return line.substr(start, end - start);
}

void searchByTopic(const string& topic, const string& inputFile) {

    ifstream file(inputFile);
    if (!file.is_open()) {
        cout << "Error: Cannot open XML file.\n";
        return;
    }

    string line;
    bool insidePost = false;
    bool matched = false;
    string postData;

    while (getline(file, line)) {

        if (line.find("<post>") != string::npos) {
            insidePost = true;
            matched = false;
            postData.clear();
        }

        if (insidePost)
            postData += line + "\n";

        if (line.find("<topic>") != string::npos) {
            string currentTopic =
                extractBetween(line, "<topic>", "</topic>");

            if (currentTopic == topic)
                matched = true;
        }

        if (line.find("</post>") != string::npos) {
            if (matched) {
                cout << "Matched Post:\n";
                cout << postData << endl;
            }
            insidePost = false;
        }
    }

    file.close();
}


