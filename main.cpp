//
// Created by Nouran Atef on 01/12/2025.
//
#include <QApplication>
#include <QPushButton>

#include "GUI/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}