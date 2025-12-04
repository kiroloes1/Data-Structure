#include "XmlTree.h"
#include <stack>

XmlTree::XmlTree() : root(nullptr) {}

XmlTree::~XmlTree() {
    if (root) delete root;
}


std::vector<std::string> XmlTree::tokenize(const std::string& xmlText) {
    std::vector<std::string> tokens;
    std::string buffer = "";

    for (size_t i = 0; i < xmlText.length(); ++i) {
        char c = xmlText[i];
        if (c == '<') {
            if (!buffer.empty()) {
                if (buffer.find_first_not_of(" \n\r\t") != std::string::npos) {
                    tokens.push_back(buffer);
                }
                buffer = "";
            }
            std::string tag = "<";
            size_t j = i + 1;
            while(j < xmlText.length() && xmlText[j] != '>') {
                tag += xmlText[j];
                j++;
            }
            if (j < xmlText.length()) tag += ">";
            tokens.push_back(tag);
            i = j;
        } else {
            buffer += c;
        }
    }
    return tokens;
}


bool XmlTree::isOpenTag(std::string tag) {
    if (tag.length() < 2) return false;
    if (tag[0] == '<' && tag[1] != '/' && tag[tag.length() - 1] == '>') {
        return true;
    }
    return false;
}

bool XmlTree::isClosingTag(std::string tag) {
    if (tag.length() < 2) return false;
    if (tag[0] == '<' && tag[1] == '/' && tag[tag.length() - 1] == '>') {
        return true;
    }
    return false;
}

std::string XmlTree::extractTag(std::string tag) {
    std::string tagBody = "";
    size_t i = 1;
    if (isClosingTag(tag)) {
        i = 2;
    }
    for (; i < tag.length() - 1; i++) {
        tagBody += tag[i];
    }
    return tagBody;
}


Node* XmlTree::buildXMLTree(const std::vector<std::string>& tokens) {
    std::stack<Node*> nodeStack;

    if (root) { delete root; root = nullptr; }

    for (const std::string& token : tokens) {
        if (isOpenTag(token)) {
            Node* newNode = new Node();
            newNode->tagName = extractTag(token);

            if (!nodeStack.empty()) {
                nodeStack.top()->children.push_back(newNode);
                newNode->parent = nodeStack.top();
            } else {
                root = newNode;
            }
            nodeStack.push(newNode);
        }
        else if (isClosingTag(token)) {
            if (!nodeStack.empty()) {
                nodeStack.pop();
            }
        }
        else {
            if (!nodeStack.empty()) {
                nodeStack.top()->textBody = token;
            }
        }
    }
    return root;
}


Node* XmlTree::parseXML(const std::string& xmlText) {
    std::vector<std::string> tokens = tokenize(xmlText);
    return buildXMLTree(tokens);
}

void XmlTree::printTree(Node* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) std::cout << "  ";

    std::cout << "<" << node->tagName << ">";

    if (!node->textBody.empty()) {
        std::cout << " : " << node->textBody;
    }
    std::cout << std::endl;

    for (Node* child : node->children) {
        printTree(child, depth + 1);
    }

    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "</" << node->tagName << ">" << std::endl;
}