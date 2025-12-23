#include "CLI/CommandLine.h"
#include "GUI/mainwindow.h"
#include <QApplication>
#include <iostream>

// --- INCLUDE FOR TESTING ---
#include "Logic/Tree/XmlTree.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  // Check for explicit GUI request
  // Usage: ./xml_editor gui
  if (argc > 1 && std::string(argv[1]) == "gui") {
    std::cout << "=== GUI Mode Activated ===" << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
  }

  // Default: Interactive CLI Mode
  // If no arguments provided, start REPL loop.
  if (argc == 1) {
    CommandLine::runInteractive();
    return 0;
  }

  std::cout << "=== CLI Single Command Mode ===" << std::endl;
  CommandLine cli(argc, argv);
  return cli.execute();
}
