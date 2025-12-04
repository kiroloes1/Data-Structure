#include "tree.h"
#include <stack>
#include <iostream>
using namespace std;

bool isOpen(string t){
    return t[0]=='<' && t[1]!='/' && t.back()=='>';
}

bool isClose(string t){
    return t[0]=='<' && t[1]=='/' && t.back()=='>';
}

string extractTag(string t){
    int start = (t[1]=='/')?2:1;
    return t.substr(start, t.length()-start-1);
}

Node* buildTree(const vector<string>& tokens){
    stack<Node*> st;
    Node* root = nullptr;

    for(const string& tk : tokens){
        if(isOpen(tk)){
            Node* node = new Node();
            node->tagName = extractTag(tk);

            if(!st.empty())
                st.top()->children.push_back(node);
            else
                root = node;

            st.push(node);
        }
        else if(isClose(tk)){
            st.pop();
        }
        else{
            if(!st.empty())
                st.top()->text = tk;
        }
    }

    return root;
}

void printTree(Node* node, int depth){
    if(!node) return;

    for(int i=0; i<depth; i++) cout << "  ";

    cout << "<" << node->tagName << ">";
    if(!node->text.empty()) cout << " : " << node->text;
    cout << endl;

    for(Node* child : node->children){
        printTree(child, depth+1);
    }

    for(int i=0; i<depth; i++) cout << "  ";
    cout << "</" << node->tagName << ">" << endl;
}
