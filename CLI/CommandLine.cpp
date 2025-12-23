#include "CommandLine.h"
#include "../Logic/Analysis/SuggestionEngine.h"
#include "../Logic/Analysis/TopicSearch.h"
#include "../Logic/Analysis/mostInfluencerId.h"
#include "../Logic/Analysis/most_active.h"
#include "../Logic/Analysis/mutualFollowers.h"
#include "../Logic/Graph/graphParser.h"
#include "../Logic/JSON/JsonConverter.h"
#include "../Logic/Services/Compression/CompressionService.h"
#include "../Logic/Services/Compression/decompression.h"
#include "../Logic/Services/Formatting/XmlFormatter.h"
#include "../Logic/Services/Graph/GraphVisualizer.h"
#include "../Logic/Services/Minification/XmlMinifier.h"
#include "../Logic/Services/Validation/XmlValidator.h"

#include "../Logic/Tree/XmlTree.h"

#include <fstream>
#include <iostream>
#include <sstream>

CommandLine::CommandLine(int argc, char *argv[]) {
  std::vector<std::string> argsVec;
  for (int i = 0; i < argc; i++)
    argsVec.push_back(argv[i]);
  parseInternal(argsVec);
}

CommandLine::CommandLine(const std::vector<std::string> &argsInput) {
  parseInternal(argsInput);
}

void CommandLine::parseInternal(const std::vector<std::string> &rawArgs) {
  if (rawArgs.size() > 0) {
    programName = rawArgs[0];
  }

  // Parse command and options
  for (size_t i = 1; i < rawArgs.size(); i++) {
    std::string arg = rawArgs[i];

    if (arg[0] == '-') {
      // It's an option (e.g. -i, -o, -f, -w, -t)
      if (i + 1 < rawArgs.size() && rawArgs[i + 1][0] != '-') {
        options[arg] = rawArgs[i + 1];
        i++; // Skip next argument as it is the value
      } else {
        options[arg] = "true"; // Boolean flag
      }
    } else {
      // It's the main command (e.g. verify, compress)
      if (args.empty())
        args.push_back(arg);
    }
  }
}

void CommandLine::runInteractive() {
  std::cout << "==========================================" << std::endl;
  std::cout << "      XML Editor Interactive Shell        " << std::endl;
  std::cout << "==========================================" << std::endl;
  std::cout << "Type commands just like in CLI mode (without './xml_editor')."
            << std::endl;
  std::cout << "Example: verify -i sample.xml" << std::endl;
  std::cout << "Type 'exit' or 'quit' to close." << std::endl;
  std::cout << "==========================================" << std::endl;

  while (true) {
    std::cout << "\nXML> ";
    std::string line;
    if (!std::getline(std::cin, line))
      break;

    if (line == "exit" || line == "quit")
      break;
    if (line.empty())
      continue;

    // Tokenize line respecting quotes could be complex.
    // For simplicity, let's split by space.
    // Improvement: Handle quotes later if needed.
    std::vector<std::string> tokens;
    tokens.push_back("xml_editor"); // Dummy program name (argv[0])

    std::stringstream ss(line);
    std::string token;
    bool firstToken = true;
    while (ss >> token) {
      // If user types "xml_editor verify ..." or "./xml_editor verify ..." in
      // shell, we should ignore the "xml_editor" part so the command becomes
      // "verify".
      if (firstToken && (token == "xml_editor" || token == "./xml_editor")) {
        firstToken = false;
        continue;
      }
      tokens.push_back(token);
      firstToken = false;
    }

    // Attempt execution if we have a command
    if (tokens.size() > 1) {
      CommandLine cli(tokens);
      cli.execute();
    }
  }
}

bool CommandLine::hasOption(const std::string &option) {
  return options.find(option) != options.end();
}

std::string CommandLine::getOption(const std::string &option) {
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
  std::cout << "XML Editor CLI" << std::endl;
  std::cout << "Usage: xml_editor <action> [options]\n" << std::endl;
  std::cout << "Actions:" << std::endl;
  std::cout << "  gui              Launch Graphical Interface" << std::endl;
  std::cout << "  verify           Check XML consistency (use -f to fix)"
            << std::endl;
  std::cout << "  format           Prettify XML indentation" << std::endl;
  std::cout << "  json             Convert XML to JSON" << std::endl;
  std::cout << "  mini             Minify XML file" << std::endl;
  std::cout << "  compress         Compress XML file" << std::endl;
  std::cout << "  decompress       Decompress .comp file" << std::endl;
  std::cout << "  draw             Draw Network Graph (to JPG)" << std::endl;
  std::cout << "  most_active      Find most active user" << std::endl;
  std::cout << "  most_influencer  Find user with most followers" << std::endl;
  std::cout << "  mutual           Find mutual followers (-ids 1,2,3)"
            << std::endl;
  std::cout << "  suggest          Suggest users to follow (-id 1)"
            << std::endl;
  std::cout << "  search           Search posts (-w word or -t topic)"
            << std::endl;
  std::cout << "\nOptions:" << std::endl;
  std::cout << "  -i <file>        Input file" << std::endl;
  std::cout << "  -o <file>        Output file" << std::endl;
  std::cout << "  -f               Fix errors (verify mode)" << std::endl;
  std::cout << "  -id <id>         User ID" << std::endl;
  std::cout << "  -ids <list>      Comma separated IDs" << std::endl;
  std::cout << "  -w <word>        Search word" << std::endl;
  std::cout << "  -t <topic>       Search topic" << std::endl;
}

// Helper to read file
std::string readFileContent(const std::string &path) {
  std::ifstream in(path);
  if (!in)
    return "";
  std::stringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

// ---------------------------------------------------------
// IMPLEMENTATIONS
// ---------------------------------------------------------

int CommandLine::handleGuiCommand() {
  std::cout << "Launching GUI..." << std::endl;
  // Use system call to restart application in GUI mode
  // This avoids QApplication lifecycle issues within the REPL loop
  int result = system("./xml_editor gui");
  return result;
}

int CommandLine::handleDrawCommand() {
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor draw -i input.xml -o output.jpg"
              << std::endl;
    return 1;
  }

  GraphVisualizer visualizer;
  if (visualizer.drawGraph(inputFile, outputFile)) {
    std::cout << "Graph drawn to " << outputFile << std::endl;
    return 0;
  }
  return 1;
}

int CommandLine::handleValidateCommand() {
  // verify -i input.xml [-f] [-o output.xml]
  std::string inputFile = getOption("-i");
  bool fix = hasOption("-f");
  std::string outputFile = getOption("-o");

  if (inputFile.empty()) {
    std::cerr << "Usage: xml_editor verify -i input.xml [-f -o output.xml]"
              << std::endl;
    return 1;
  }

  std::string content = readFileContent(inputFile);
  if (content.empty()) {
    std::cerr << "Error: Could not read file " << inputFile << std::endl;
    return 1;
  }

  XmlValidator validator;
  std::string errors;
  bool isValid = validator.validate(content, errors);

  if (isValid) {
    std::cout << "valid" << std::endl;
    return 0;
  } else {
    std::cout << "invalid" << std::endl;
    std::cout << errors << std::endl;

    if (fix) {
      if (outputFile.empty()) {
        std::cerr << "Error: Output file required for fix (-o output.xml)"
                  << std::endl;
        return 1;
      }
      std::string fixed = validator.fix(content);
      std::ofstream out(outputFile);
      out << fixed;
      out.close();
      std::cout << "Fixed XML saved to " << outputFile << std::endl;
    }
    return 1;
  }
}

int CommandLine::handleFormatCommand() {
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor format -i input.xml -o output.xml"
              << std::endl;
    return 1;
  }

  std::string content = readFileContent(inputFile);
  if (content.empty())
    return 1;

  try {
    XmlFormatter formatter;
    std::string prettified = formatter.prettify_xml(content);
    std::ofstream out(outputFile);
    out << prettified;
    out.close();
    std::cout << "Formatted XML saved to " << outputFile << std::endl;
  } catch (...) {
    std::cerr << "Error formatting file." << std::endl;
    return 1;
  }
  return 0;
}

int CommandLine::handleMinifyCommand() {
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor mini -i input.xml -o output.xml"
              << std::endl;
    return 1;
  }

  minifyXML(inputFile, outputFile);
  return 0;
}

int CommandLine::handleConvertCommand() {
  // known as 'json' command
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor json -i input.xml -o output.json"
              << std::endl;
    return 1;
  }

  std::string content = readFileContent(inputFile);
  if (content.empty())
    return 1;

  XmlTree tree;
  Node *root = tree.parseXML(content);
  if (!root) {
    std::cerr << "Error: Invalid XML, cannot convert to JSON." << std::endl;
    return 1;
  }

  std::string json = xmlToJSON(root);
  std::ofstream out(outputFile);
  out << json;
  out.close();
  std::cout << "JSON saved to " << outputFile << std::endl;

  // Clean up if needed (Node destructor handles children)
  // delete root; // Removed: XmlTree destructor already manages this memory.
  return 0;
}

int CommandLine::handleCompressCommand() {
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor compress -i input.xml -o output.comp"
              << std::endl;
    return 1;
  }

  compressXML(inputFile, outputFile);
  return 0;
}

int CommandLine::handleDecompressCommand() {
  std::string inputFile = getOption("-i");
  std::string outputFile = getOption("-o");

  if (inputFile.empty() || outputFile.empty()) {
    std::cerr << "Usage: xml_editor decompress -i input.comp -o output.xml"
              << std::endl;
    return 1;
  }

  if (decompressFile(inputFile, outputFile)) {
    std::cout << "Decompressed to " << outputFile << std::endl;
    return 0;
  } else {
    std::cerr << "Error decompressing file." << std::endl;
    return 1;
  }
}

// ---------------------------------------------------------
// GRAPH ANALYSIS HANDLERS
// ---------------------------------------------------------
// Helper to load graph
bool loadGraphHelper(const std::string &inputFile, Graph &g,
                     std::map<int, std::string> &names,
                     std::map<int, std::vector<Post>> &posts) {
  if (inputFile.empty()) {
    std::cerr << "Error: Input file required (-i)" << std::endl;
    return false;
  }
  try {
    loadXMLtoGraph(inputFile, g, names, posts);
    return true;
  } catch (...) {
    std::cerr << "Error processing XML for Graph Analysis." << std::endl;
    return false;
  }
}

int CommandLine::handleMostActiveCommand() {
  std::string inputFile = getOption("-i");
  Graph g;
  std::map<int, std::string> names;
  std::map<int, std::vector<Post>> posts;
  if (!loadGraphHelper(inputFile, g, names, posts))
    return 1;

  std::vector<int> activeUsers = most_active_user(g);

  if (activeUsers.empty()) {
    std::cout << "No active users found." << std::endl;
    return 0;
  }

  std::cout << "Most Active User(s):" << std::endl;
  for (int id : activeUsers) {
    std::string name = (names.count(id) ? names[id] : "Unknown");
    std::cout << "- " << name << " (ID: " << id << ")" << std::endl;
  }
  return 0;
}

int CommandLine::handleMostInfluencerCommand() {
  std::string inputFile = getOption("-i");
  Graph g;
  std::map<int, std::string> names;
  std::map<int, std::vector<Post>> posts;
  if (!loadGraphHelper(inputFile, g, names, posts))
    return 1;

  print_influencer_result(g, names);
  return 0;
}

int CommandLine::handleMutualCommand() {
  std::string inputFile = getOption("-i");
  std::string idsStr = getOption("-ids");
  if (idsStr.empty()) {
    std::cerr << "Usage: xml_editor mutual -i input.xml -ids 1,2,3"
              << std::endl;
    return 1;
  }

  Graph g;
  std::map<int, std::string> names;
  std::map<int, std::vector<Post>> posts;
  if (!loadGraphHelper(inputFile, g, names, posts))
    return 1;

  std::vector<int> userIds;
  std::stringstream ss(idsStr);
  std::string segment;
  while (std::getline(ss, segment, ',')) {
    if (!segment.empty())
      userIds.push_back(std::stoi(segment));
  }

  // Functional API call
  std::set<int> mutualsSet = get_mutual_followers(g, userIds);
  std::vector<int> mutuals(mutualsSet.begin(), mutualsSet.end());

  if (mutuals.empty())
    std::cout << "No mutual followers found." << std::endl;
  else {
    std::cout << "Mutual Followers: ";
    for (size_t i = 0; i < mutuals.size(); ++i) {
      std::string name =
          (names.count(mutuals[i]) ? names[mutuals[i]] : "Unknown");
      std::cout << name << " (ID:" << mutuals[i] << ")"
                << (i == mutuals.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
  }
  return 0;
}

int CommandLine::handleSuggestCommand() {
  std::string inputFile = getOption("-i");
  std::string idStr = getOption("-id");
  if (idStr.empty()) {
    std::cerr << "Usage: xml_editor suggest -i input.xml -id 1" << std::endl;
    return 1;
  }
  int userId = std::stoi(idStr);

  Graph g;
  std::map<int, std::string> names;
  std::map<int, std::vector<Post>> posts;
  if (!loadGraphHelper(inputFile, g, names, posts))
    return 1;

  // Functional API call
  std::vector<int> suggestions = suggest_users(userId, g);

  if (suggestions.empty())
    std::cout << "No suggestions available." << std::endl;
  else {
    std::cout << "Suggestions for User " << userId << ":" << std::endl;
    for (int id : suggestions) {
      // For suggestions, we usually want name
      std::string name = (names.count(id) ? names[id] : "Unknown");
      std::cout << "- " << name << " (ID: " << id << ")" << std::endl;
    }
  }
  return 0;
}

int CommandLine::handleSearchCommand() {
  std::string inputFile = getOption("-i");
  std::string word = getOption("-w");
  std::string topic = getOption("-t");

  if (inputFile.empty() || (word.empty() && topic.empty())) {
    std::cerr << "Usage: xml_editor search -i input.xml -w word OR -t topic"
              << std::endl;
    return 1;
  }

  Graph g;
  std::map<int, std::string> names;
  std::map<int, std::vector<Post>> posts;
  if (!loadGraphHelper(inputFile, g, names, posts))
    return 1;

  if (!topic.empty()) {
    searchByTopic(topic, posts, names);
  } else {
    searchByWord(word, posts, names);
  }
  return 0;
}

int CommandLine::execute() {
  std::string command = getCommand();

  if (command.empty() || hasOption("-h") || hasOption("--help")) {
    printUsage();
    return 0;
  }

  if (command == "gui")
    return handleGuiCommand();
  if (command == "draw")
    return handleDrawCommand();
  if (command == "verify")
    return handleValidateCommand();
  if (command == "format")
    return handleFormatCommand();
  if (command == "mini")
    return handleMinifyCommand();
  if (command == "json")
    return handleConvertCommand();
  if (command == "compress")
    return handleCompressCommand();
  if (command == "decompress")
    return handleDecompressCommand();

  if (command == "most_active")
    return handleMostActiveCommand();
  if (command == "most_influencer")
    return handleMostInfluencerCommand();
  if (command == "mutual")
    return handleMutualCommand();
  if (command == "suggest")
    return handleSuggestCommand();
  if (command == "search")
    return handleSearchCommand();

  std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
  printUsage();
  return 1;
}
