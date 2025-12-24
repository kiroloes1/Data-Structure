#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <stack>
#include <string>
#include <vector>

struct Tag {
  std::string name;
  bool isClosing;
  std::string fullContent;
  int lineIndex;
};

class XmlValidator {
public:
  bool validate(const std::string &xmlContent, std::string &errorLog);

  std::string fix(const std::string &xmlContent);

private:
  bool parseTags(const std::string &xmlContent, std::vector<Tag> &tags,
                 std::string &errorLog);
};

#endif