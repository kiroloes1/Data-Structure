#include "validator.h"
#include <stack>
#include <iostream>
using namespace std;

bool isOpenTag(const string& t){
    return t[0]=='<' && t[1]!='/' && t.back()=='>';
}

bool isCloseTag(const string& t){
    return t[0]=='<' && t[1]=='/' && t.back()=='>';
}

string getTagName(const string& t){
    int start = (t[1]=='/') ? 2 : 1;
    return t.substr(start, t.length()-start-1);
}

bool validateXML(const vector<string>& tokens){
    stack<string> st;
    int i = -1;

    for(const string& tk : tokens){
        i++;

        if(isOpenTag(tk)){
            st.push(getTagName(tk));
        }
        else if(isCloseTag(tk)){
            string tag = getTagName(tk);

            if(st.empty()){
                cout << "Error: Closing tag without opening (" << tag << ")\n";
                return false;
            }

            if(st.top() != tag){
                cout << "Error: Expected </" << st.top() 
                     << "> but found </" << tag << ">\n";
                return false;
            }

            st.pop();
        }
    }

    if(!st.empty()){
        cout << "Error: Missing closing tag for <" << st.top() << ">\n";
        return false;
    }

    return true;
}
