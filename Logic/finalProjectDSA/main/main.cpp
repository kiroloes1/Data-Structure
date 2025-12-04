#include <iostream>
#include "../extract/extract.h"
#include "../validator/validator.h"
#include "../tree/tree.h"

using namespace std;

int main(){
    vector<string> tokens = extractXMLTokens("sample.xml");

    cout << "Tokens Extracted = " << tokens.size() << endl;

    if(validateXML(tokens)){
        cout << "\nXML IS VALID\n\n";

        Node* root = buildTree(tokens);
        printTree(root);
    }
    else{
        cout << "\nXML IS INVALID\n";
    }

    return 0;
}
