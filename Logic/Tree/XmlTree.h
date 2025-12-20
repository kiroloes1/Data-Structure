#ifndef XMLTREE_H
#define XMLTREE_H

#include <string>
#include <vector>
#include <iostream>

struct Node {
    std::string tagName;
    std::string textBody;
    std::vector<Node*> children;
     Node* parent;

    Node(std::string name = "", Node* p = nullptr)
        : tagName(name), parent(p), textBody("") {}

    ~Node() {
        for (Node* c : children) delete c;
    }
};

class XmlTree {
public:
    XmlTree();
    ~XmlTree();

    Node* parseXML(const std::string& xmlText);

    void printTree(Node* root, int depth = 0);

private:
    Node* root;

    std::vector<std::string> tokenize(const std::string& xmlText);

    bool isOpenTag(std::string tag);
    bool isClosingTag(std::string tag);
    std::string extractTag(std::string tag);

    Node* buildXMLTree(const std::vector<std::string>& tokens);
};

#endif