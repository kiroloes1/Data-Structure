#include "extract.h"
#include <fstream>
#include <iostream>
using namespace std;

string trimSpaces(const string& s){
    string out="";
    for(char c : s){
        if(c!=' ') out += c;
    }
    return out;
}

string readFile(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Cannot open file\n";
        return "";
    }
    string xml="", line;
    while(getline(file, line)) xml += line;
    return trimSpaces(xml);
}

vector<string> tokenizeXML(const string& xml){
    vector<string> tokens;
    string sub="";
    int i = 0;

    while(i < xml.length()){
        if(xml[i] == '<'){
            sub = "<";
            i++;
            while(i < xml.length() && xml[i] != '>'){
                sub += xml[i];
                i++;
            }
            sub += ">";
            tokens.push_back(sub);
            i++;
        }
        else {
            sub = "";
            while(i < xml.length() && xml[i] != '<'){
                sub += xml[i];
                i++;
            }
            if(!sub.empty()) tokens.push_back(sub);
        }
    }

    return tokens;
}

vector<string> extractXMLTokens(const string& filename){
    string xml = readFile(filename);
    return tokenizeXML(xml);
}
