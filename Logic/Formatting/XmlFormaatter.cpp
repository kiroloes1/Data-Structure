#include <iostream>
#include <fstream>
#include "XmlFormatter.h" 

using namespace std;

int main() {
    //Example 
    string xml = R"(
<social>
<user><id>1</id><name>Aly</name></user>
<user><id>2</id><name>Mona</name></user>
</social>
)";

    string formatted = prettify_xml(xml);

    cout << formatted << endl;

    ofstream outfile("output.xml");
    outfile << formatted;
    outfile.close();

    return 0;
}
