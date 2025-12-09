#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include <string>
#include <vector>

class XmlFormatter {
public:
    std::string prettify_xml(const std::string& xml);

private:
    std::vector<std::string> tokenize(const std::string& xml);
};

#endif