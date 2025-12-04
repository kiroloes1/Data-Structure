#include "JsonConverter.h"
#include <map>
#include <iostream>

using namespace std;

string indent(int level) {
    return string(level * 4, ' ');
}

string nodeToJSONBody(Node* node, int level) {
    if (node->children.empty()) {
        return "\"" + node->innerText + "\"";
    }

    string json = "{\n";

    map<string, vector<Node*>> groups;
    for (Node* child : node->children) {
        groups[child->tagName].push_back(child);
    }

    int count = 0;
    for (auto const& [key, nodes] : groups) {
        if (count > 0) json += ",\n";

        json += indent(level + 1) + "\"" + key + "\": ";

        if (nodes.size() > 1) {
            json += "[\n";
            for (size_t i = 0; i < nodes.size(); ++i) {
                if (i > 0) json += ",\n";


                json += indent(level + 2) + nodeToJSONBody(nodes[i], level + 2);
            }
            json += "\n" + indent(level + 1) + "]";
        }

        else {

            json += nodeToJSONBody(nodes[0], level + 1);
        }
        count++;
    }


    json += "\n" + indent(level) + "}";
    return json;
}

string xmlToJSON(Node* root) {
    if (!root) return "";

    string json = "{\n";
    json += indent(1) + "\"" + root->tagName + "\": " + nodeToJSONBody(root, 1);
    json += "\n}";

    return json;
}
