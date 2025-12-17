#include <QApplication>
#include "GUI/mainwindow.h"
#include "CLI/CommandLine.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // Check if running in CLI mode (has command line arguments)
    if (argc > 1) {
        // CLI Mode
        std::cout << "=== CLI Mode Activated ===" << std::endl;
        std::cout << "Arguments: " << argc << std::endl;
        for (int i = 0; i < argc; i++) {
            std::cout << "  argv[" << i << "]: " << argv[i] << std::endl;
        }
        std::cout << std::endl;

        CommandLine cli(argc, argv);
        int result = cli.execute();

        std::cout << "=== CLI Execution Complete (exit code: " << result << ") ===" << std::endl;
        return result;
    } else {
        // GUI Mode
        std::cout << "=== GUI Mode ===" << std::endl;
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
