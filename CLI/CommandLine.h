#ifndef DATA_STRUCTURE_COMMANDLINE_H
#define DATA_STRUCTURE_COMMANDLINE_H

#include <string>
#include <vector>
#include <map>

class CommandLine {
private:
    std::string programName;
    std::vector<std::string> args;
    std::map<std::string, std::string> options;
    
    void parseArguments(int argc, char* argv[]);
    void printUsage();
    
public:
    CommandLine(int argc, char* argv[]);
    
    bool hasOption(const std::string& option);
    std::string getOption(const std::string& option);
    std::string getCommand();
    
    // Execute the command
    int execute();
    
    // Command handlers
    int handleDrawCommand();
    int handleValidateCommand();
    int handleFormatCommand();
    int handleMinifyCommand();
    int handleConvertCommand();
};

#endif // DATA_STRUCTURE_COMMANDLINE_H