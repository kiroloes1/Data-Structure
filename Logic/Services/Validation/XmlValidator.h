#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <string>
#include <vector>
#include <stack>

struct Tag {
    std::string name;
    bool isClosing;
    std::string fullContent;
    int lineIndex;
};

class XmlValidator {
public:
    bool validate(const std::string& xmlContent, std::string& errorLog);

    std::string fix(const std::string& xmlContent);

private:
    std::vector<Tag> parseTags(const std::string& xmlContent);
};

#endif