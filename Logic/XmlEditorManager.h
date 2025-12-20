#ifndef XMLEDITORMANAGER_H
#define XMLEDITORMANAGER_H

#include <string>
#include "Validation/XmlValidator.h"
#include "Tree/XmlTree.h"
#include "Formatting/XmlFormatter.h"

class XmlEditorManager {
private:
    XmlValidator validator;
    XmlTree treeBuilder;
    XmlFormatter formatter;

public:
    XmlEditorManager() {
    }

    bool verifyXml(const std::string& xmlContent, std::string& errorLog) {
        return validator.validate(xmlContent, errorLog);
    }

    std::string fixXml(const std::string& xmlContent) {
        return validator.fix(xmlContent);
    }
    std::string formatXml(const std::string& xmlContent) {
        return formatter.prettify_xml(xmlContent);
    }
};

#endif