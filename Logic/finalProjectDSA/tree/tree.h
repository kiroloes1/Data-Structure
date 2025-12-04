#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
using namespace std;

struct Node {
    string tagName;
    string text;
    vector<Node*> children;
};

Node* buildTree(const vector<string>& tokens);
void printTree(Node* root, int depth=0);

#endif
