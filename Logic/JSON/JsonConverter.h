#ifndef JSON_CONVERTER_H
#define JSON_CONVERTER_H
#include <string>
#include <vector>
// TODO : REMOVE THIS STRUCT ONCE THE SHARED NODE.H IS AVAILABLE
struct Node {
    std::string tagName;
    std::string innerText;
    std::vector<Node*> children;
    Node* parent;

    Node(std::string name, std::string text = "")
        : tagName(name), innerText(text), parent(nullptr) {}
};

std::string xmlToJSON(Node* root);

#endif
