#include "XmlValidator.h"
#include <sstream>
#include <stack>
#include <iostream>

bool isWhitespace(const std::string& str) {
    return str.find_first_not_of(" \n\r\t") == std::string::npos;
}

// ---------------------------------------------------------
// HELPER: Extract Tags
// ---------------------------------------------------------
std::vector<Tag> XmlValidator::parseTags(const std::string& xmlContent) {
    std::vector<Tag> tags;
    bool insideTag = false;
    std::string buffer = "";
    int currentLine = 1;

    for (size_t i = 0; i < xmlContent.length(); ++i) {
        char c = xmlContent[i];
        if (c == '\n') currentLine++;

        if (c == '<') {
            insideTag = true;
            buffer = "<";
        }
        else if (c == '>') {
            if (insideTag) {
                buffer += ">";
                insideTag = false;
                if (buffer.size() > 1 && (buffer[1] == '?' || buffer[1] == '!')) continue;

                Tag t;
                t.fullContent = buffer;
                t.lineIndex = currentLine;
                if (buffer.length() > 2 && buffer[1] == '/') {
                    t.isClosing = true;
                    t.name = buffer.substr(2, buffer.length() - 3);
                } else {
                    t.isClosing = false;
                    size_t spacePos = buffer.find(' ');
                    if (spacePos != std::string::npos) t.name = buffer.substr(1, spacePos - 1);
                    else t.name = buffer.substr(1, buffer.length() - 2);
                }
                tags.push_back(t);
            }
        }
        else if (insideTag) {
            buffer += c;
        }
    }
    return tags;
}

// ---------------------------------------------------------
// FUNCTION 1: VALIDATE
// ---------------------------------------------------------
bool XmlValidator::validate(const std::string& xmlContent, std::string& errorLog) {
    std::vector<Tag> tags = parseTags(xmlContent);
    std::stack<Tag> tagStack;
    std::stringstream ss;
    int errorCount = 0;

    for (const auto& tag : tags) {
        if (!tag.isClosing) {
            tagStack.push(tag);
        } else {
            if (tagStack.empty()) {
                ss << "Line " << tag.lineIndex << ": Found closing tag </" << tag.name << "> but stack is empty.\n";
                errorCount++;
            } else {
                Tag top = tagStack.top();
                if (top.name != tag.name) {
                    ss << "Line " << tag.lineIndex << ": Mismatched tag. Expected </" << top.name
                       << "> but found </" << tag.name << ">.\n";
                    errorCount++;
                } else {
                    tagStack.pop();
                }
            }
        }
    }

    while (!tagStack.empty()) {
        Tag t = tagStack.top();
        ss << "Line " << t.lineIndex << ": Tag <" << t.name << "> was opened but never closed.\n";
        tagStack.pop();
        errorCount++;
    }

    errorLog = ss.str();
    return (errorCount == 0);
}

// ---------------------------------------------------------
// FUNCTION 2:  FIX
// ---------------------------------------------------------
std::string XmlValidator::fix(const std::string& xmlContent) {
    struct FixState {
        std::string name;
        bool hasText;
    };

    std::stack<FixState> stateStack;
    std::string fixedXml = "";
    std::string buffer = "";
    bool insideTag = false;

    for (size_t i = 0; i < xmlContent.length(); ++i) {
        char c = xmlContent[i];

        if (c == '<') {
            if (!stateStack.empty() && stateStack.top().hasText) {
                if (!isWhitespace(buffer)) {
                    size_t lastChar = buffer.find_last_not_of(" \n\r\t");
                    std::string content = buffer.substr(0, lastChar + 1);
                    std::string trailingSpace = buffer.substr(lastChar + 1);

                    fixedXml += content;
                    fixedXml += "</" + stateStack.top().name + ">";
                    fixedXml += trailingSpace;

                    buffer = "";
                    stateStack.pop();
                }
            }

            fixedXml += buffer;
            buffer = "";

            std::string tagStr = "<";
            size_t j = i + 1;
            while(j < xmlContent.length() && xmlContent[j] != '>') {
                tagStr += xmlContent[j];
                j++;
            }
            if(j < xmlContent.length()) tagStr += ">";

            bool isClosing = (tagStr.length() > 2 && tagStr[1] == '/');
            std::string name;
            if (isClosing) name = tagStr.substr(2, tagStr.length() - 3);
            else {
                 size_t sp = tagStr.find(' ');
                 if(sp != std::string::npos) name = tagStr.substr(1, sp - 1);
                 else name = tagStr.substr(1, tagStr.length() - 2);
            }

            if (tagStr.length() > 1 && (tagStr[1] == '?' || tagStr[1] == '!')) {
                fixedXml += tagStr;
                i = j;
                continue;
            }

            if (!isClosing) {
                stateStack.push({name, false});
                fixedXml += tagStr;
            } else {
                if (!stateStack.empty() && stateStack.top().name == name) {
                    stateStack.pop();
                    fixedXml += tagStr;
                }
                else {
                    bool existsInStack = false;
                    std::stack<FixState> temp = stateStack;
                    while(!temp.empty()){
                        if(temp.top().name == name) { existsInStack = true; break; }
                        temp.pop();
                    }

                    if (existsInStack) {
                        while (!stateStack.empty() && stateStack.top().name != name) {
                            fixedXml += "</" + stateStack.top().name + ">";
                            stateStack.pop();
                        }
                        stateStack.pop();
                        fixedXml += tagStr;
                    }
                }
            }
            i = j;
        } else {
            buffer += c;
            if (!stateStack.empty() && !isWhitespace(std::string(1, c))) {
                stateStack.top().hasText = true;
            }
        }
    }
    fixedXml += buffer;

    while (!stateStack.empty()) {
        fixedXml += "\n</" + stateStack.top().name + ">";
        stateStack.pop();
    }

    return fixedXml;
}