#include "XmlFormatter.h"
#include <regex>
#include <iostream>

using namespace std;

vector<string> XmlFormatter::tokenize(const string& xml) {
    vector<string> tokens;
    regex re("<[^>]+>|[^<]+");
    sregex_iterator begin(xml.begin(), xml.end(), re);
    sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        string token = it->str();
        if (token.find_first_not_of(" \t\n\r") != string::npos)
            tokens.push_back(token);
    }
    return tokens;
}

string XmlFormatter::prettify_xml(const string& xml) {
    string indent_unit = "    ";
    vector<string> tokens = tokenize(xml);
    string pretty_xml;
    int indent_level = 0;

    for (size_t i = 0; i < tokens.size(); ++i) {
        string token = tokens[i];
        token = regex_replace(token, regex("^\\s+|\\s+$"), "");

        string next_token = (i + 1 < tokens.size()) ? regex_replace(tokens[i + 1], regex("^\\s+|\\s+$"), "") : "";
        string next_next_token = (i + 2 < tokens.size()) ? regex_replace(tokens[i + 2], regex("^\\s+|\\s+$"), "") : "";

        if (!token.empty() && token[0] == '<' && token[1] != '/' && token.back() != '/' &&
            !next_token.empty() && next_token[0] != '<' &&
            !next_next_token.empty() && next_next_token.substr(0, 2) == "</") {

            pretty_xml += string(indent_level * indent_unit.size(), ' ') + token + next_token + next_next_token + "\n";
            i += 2;
            continue;
        }

        if (token.substr(0, 2) == "</") {
            if(indent_level > 0) indent_level--;
            pretty_xml += string(indent_level * indent_unit.size(), ' ') + token + "\n";
        }
        else if (token.back() == '/' && token[0] == '<') {
            pretty_xml += string(indent_level * indent_unit.size(), ' ') + token + "\n";
        }
        else if (token[0] == '<') {
            pretty_xml += string(indent_level * indent_unit.size(), ' ') + token + "\n";
            indent_level++;
        }
        else {
            pretty_xml += string(indent_level * indent_unit.size(), ' ') + token + "\n";
        }
    }

    return pretty_xml;
}
