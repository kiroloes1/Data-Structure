#include "GUI/mainwindow.h"
#include <QApplication>

// --- INCLUDE FOR TESTING ---
#include "Logic/Tree/XmlTree.h"
#include <iostream>
#include <string>

void runTreeTest() {
    // 1. Create dummy XML data to test XmlTree
    std::string testXml =
        "<users>"
        "   <user>"
        "       <id>1</id>"
        "       <name>Ahmed</name>"
        "       <posts>"
        "           <post>Hello World</post>"
        "       </posts>"
        "   </user>"
        "   <user>"
        "       <id>2</id>"
        "       <name>Sarah</name>"
        "   </user>"
        "</users>";

    std::cout << "========================================" << std::endl;
    std::cout << "STARTING XML TREE TEST" << std::endl;
    std::cout << "========================================" << std::endl;
    XmlTree treeBuilder;

    Node* root = treeBuilder.parseXML(testXml);

    if (root) {
        std::cout << "Tree built successfully! Printing structure:" << std::endl;
        treeBuilder.printTree(root);
    } else {
        std::cout << "❌ Error: Root is null (Parsing failed)." << std::endl;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "TEST FINISHED" << std::endl;
    std::cout << "========================================" << std::endl;
}

int main(int argc, char *argv[])
{
    runTreeTest();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}