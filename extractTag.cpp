#include "stack.cpp"
#include <vector>
#include <string>
#include <fstream>


// to remove space (start , end)
string trim(string file){
    string fileWithoutSpace="";
    for(int i=0; i<file.length();i++){
        if(file[i]!=' '){
            fileWithoutSpace+=file[i];
        }
    }
    return fileWithoutSpace;
}


// convert XML to String
string readXMLFile(const string& filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return "";
    }

    string xmlText, line;
    while(getline(file, line)) {
        xmlText += line ; 
        
    }

    file.close();
    return trim(xmlText);
}


// convert XML TO Vector 
vector<string> convertXMLToVector(const string& xmlString) {
    vector<string> tokens;
    string subValue = "";
    size_t i = 0;

    while (i < xmlString.length()) {
        if (xmlString[i] == '<') {
            subValue = "<";
            i++;
            while (i < xmlString.length() && xmlString[i] != '>') {
                subValue += xmlString[i];
                i++;
            }
            if (i < xmlString.length() && xmlString[i] == '>') {
                subValue += '>';
                tokens.push_back(subValue);
                i++;
            }
        } else {
            subValue = "";
            while (i < xmlString.length() && xmlString[i] != '<') {
                subValue += xmlString[i];
                i++;
            }
            if (!subValue.empty()) {
                tokens.push_back(subValue);
            }
        }
    }

    return tokens;
}


int main() {
    string xmlContentAsString =readXMLFile("sample.xml");
   
   vector<string> tokens= convertXMLToVector(xmlContentAsString);

    for(const auto& token : tokens) {
        cout << token << endl;
    }
   cout<<"elementOne"<<tokens[tokens.size()-1];

}