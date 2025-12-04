#ifndef XMLEDITORMANAGER_H
#define XMLEDITORMANAGER_H

#include <string>
#include "Validation/XmlValidator.h"
#include "Tree/XmlTree.h"

class XmlEditorManager {
private:
    XmlValidator validator;
    XmlTree treeBuilder;

public:
    XmlEditorManager() {
    }

    bool verifyXml(const std::string& xmlContent, std::string& errorLog) {
        return validator.validate(xmlContent, errorLog);
    }

    std::string fixXml(const std::string& xmlContent) {
        return validator.fix(xmlContent);
    }
};

#endif