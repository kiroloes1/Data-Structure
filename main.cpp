#include <iostream>
#include "decompression.h"
#include "decompression.cpp"
using namespace std;

int main() {
    if (decompressFile("data.comp", "output.xml")) {
        cout << "Decompression successful! Output saved in output.xml\n";
    } else {
        cout << "Decompression failed.\n";
    }
    return 0;
}
