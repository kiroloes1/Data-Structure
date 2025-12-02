#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include"extractTag.h"


//validation =>Mohamed Gamal

//function to extract tag name from a tag string
string getTagName(const string& tag){
    int start=(tag[1]=='/')? 2 : 1 ;
    int end=tag.find('>');
    return tag.substr(start,end-start);
}

//function to check if a tag is an opening tag
bool isOpeningTag(const string& tag){
    return tag.size() >= 3 && tag[1]!='/';
}

//function to check if a tag is a closing tag
bool isClosingTag(const string& tag){
    return tag.size() >= 3 && tag[1] =='/';
}

//function to validate XML structure using stack
bool validateXML(vector<string>& tokens){
    stack<string> sta;
    int counter=-1;
    for(const string& token : tokens){
        counter++;
        if(token[0]=='<'){
            if(isOpeningTag(token)){
                string tag = getTagName(token);
                sta.push(tag);
            }
            else if (isClosingTag(token))
            {
                string tag = getTagName(token);
                if (sta.empty())
                {
                    cout<<"Error in founding closing tag!"<< tag <<"has no opening tag"<<endl;
                    cout<<"error at token index: "<<counter<<endl;
                    return false;
                }
                if (sta.top()!=tag)
                {
                    cout << "Error: Expected closing for <" << sta.top() << "> but found </" << tag << ">\n";
                    cout<<"error at token index: "<<counter<<endl;
                    return false;
                }

                sta.pop();
            }
            
        }
    }

    if(!sta.empty()){
         cout << "Error: Missing closing tag for <" << sta.top() << ">\n";
         cout<<"error at token index: "<<counter<<endl;
        return false;
    }
    
    return true;
}


int main() {
    string xmlContentAsString =readXMLFile("sample.xml");
   vector<string> tokens= convertXMLToVector(xmlContentAsString);

     if (validateXML(tokens))
        cout << "XML is VALID\n";
    else
        cout << "XML is INVALID\n";
    return 0;

}