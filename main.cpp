#include "GUI/mainwindow.h"
#include <QApplication>

// --- INCLUDE FOR TESTING ---
#include "Logic/Tree/XmlTree.h"
#include <iostream>
#include <string>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}