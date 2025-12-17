#include "CommandLine.h"
#include "../Logic/Services/Graph/GraphVisualizer.h"
#include <iostream>

CommandLine::CommandLine(int argc, char* argv[]) {
    parseArguments(argc, argv);
}

void CommandLine::parseArguments(int argc, char* argv[]) {
    if (argc > 0) {
        programName = argv[0];
    }

    // Parse command and options
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg[0] == '-') {
            // It's an option
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options[arg] = argv[i + 1];
                i++; // Skip next argument
            } else {
                options[arg] = "true";
            }
        } else {
            // It's a command or argument
            args.push_back(arg);
        }
    }
}

bool CommandLine::hasOption(const std::string& option) {
    return options.find(option) != options.end();
}

std::string CommandLine::getOption(const std::string& option) {
    if (hasOption(option)) {
        return options[option];
    }
    return "";
}

std::string CommandLine::getCommand() {
    if (!args.empty()) {
        return args[0];
    }
    return "";
}

void CommandLine::printUsage() {
    std::cout << "XML Editor - Command Line Interface\n" << std::endl;
    std::cout << "Usage: " << programName << " <command> [options]\n" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  draw     Draw XML social network graph" << std::endl;
    std::cout << "  validate   Validate XML file" << std::endl;
    std::cout << "  format     Format (prettify) XML file" << std::endl;
    std::cout << "  minify     Minify XML file" << std::endl;
    std::cout << "  convert    Convert XML to JSON" << std::endl;
    std::cout << "\nOptions for 'draw' command:" << std::endl;
    std::cout << "  -i <file>  Input XML file" << std::endl;
    std::cout << "  -o <file>  Output image file (JPG)" << std::endl;
    std::cout << "\nExample:" << std::endl;
    std::cout << "  " << programName << " draw -i input_file.xml -o output_file.jpg" << std::endl;
}

int CommandLine::handleDrawCommand() {
    std::string inputFile = getOption("-i");
    std::string outputFile = getOption("-o");

    // --- DEBUG ADDITION ---
    std::cout << "DEBUG 1: Command handler entered." << std::endl;
    std::cout.flush(); // Force this line to display immediately
    // ----------------------

    if (inputFile.empty() || outputFile.empty()) {
        std::cerr << "Error: Missing required options for 'draw' command" << std::endl;
        std::cerr << "Usage: draw -i <input_file.xml> -o <output_file.jpg>" << std::endl;
        return 1;
    }

    std::cout << "Drawing graph from: " << inputFile << std::endl;
    std::cout << "Output will be saved to: " << outputFile << std::endl;

    // --- DEBUG ADDITION ---
    std::cout << "DEBUG 2: Calling GraphVisualizer..." << std::endl;
    std::cout.flush(); // Force this line to display immediately
    // ----------------------

    GraphVisualizer visualizer;
    bool success = visualizer.drawGraph(inputFile, outputFile);

    if (success) {
        std::cout << "Graph visualization completed successfully!" << std::endl;
        return 0;
    } else {
        std::cerr << "Error: Failed to generate graph visualization" << std::endl;
        return 1;
    }
}

int CommandLine::handleValidateCommand() {
    std::cout << "Validate command not yet implemented" << std::endl;
    return 1;
}

int CommandLine::handleFormatCommand() {
    std::cout << "Format command not yet implemented" << std::endl;
    return 1;
}

int CommandLine::handleMinifyCommand() {
    std::cout << "Minify command not yet implemented" << std::endl;
    return 1;
}

int CommandLine::handleConvertCommand() {
    std::cout << "Convert command not yet implemented" << std::endl;
    return 1;
}

int CommandLine::execute() {
    std::string command = getCommand();

    if (command.empty() || hasOption("-h") || hasOption("--help")) {
        printUsage();
        return 0;
    }

    if (command == "draw") {
        return handleDrawCommand();
    } else if (command == "validate") {
        return handleValidateCommand();
    } else if (command == "format") {
        return handleFormatCommand();
    } else if (command == "minify") {
        return handleMinifyCommand();
    } else if (command == "convert") {
        return handleConvertCommand();
    } else {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        printUsage();
        return 1;
    }
}
