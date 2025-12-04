#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include <string>
#include <vector>

using namespace std;


vector<string> tokenize(const string& xml);
string prettify_xml(const string& xml, const string& indent_unit = "    ");

#endif 
