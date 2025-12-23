#ifndef DATA_STRUCTURE_COMMANDLINE_H
#define DATA_STRUCTURE_COMMANDLINE_H

#include <map>
#include <string>
#include <vector>

class CommandLine {
private:
  std::string programName;
  std::vector<std::string> args;
  std::map<std::string, std::string> options;

  void printUsage();

public:
  CommandLine(int argc, char *argv[]);
  CommandLine(const std::vector<std::string> &args);

  static void runInteractive();

  bool hasOption(const std::string &option);
  std::string getOption(const std::string &option);
  std::string getCommand();

  // Execute the command
  int execute();

private:
  void parseInternal(const std::vector<std::string> &rawArgs);

  // Command handlers
  int handleDrawCommand();
  int handleValidateCommand();
  int handleFormatCommand();
  int handleMinifyCommand();
  int handleConvertCommand();
  int handleCompressCommand();
  int handleDecompressCommand();

  // Analysis
  int handleMostActiveCommand();
  int handleMostInfluencerCommand();
  int handleMutualCommand();
  int handleSuggestCommand();
  int handleSearchCommand();
};

#endif // DATA_STRUCTURE_COMMANDLINE_H