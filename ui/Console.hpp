#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#if defined(__linux__)
#define CLEAR_SCREEN system("clear");
#elif _WIN32
#define CLEAR_SCREEN system("cls");
#else
#define CLEAR_SCREEN system("clear");
#endif

struct CommandResult {
  std::string output;
  bool shouldExit = false;
};

class Console {
public:
  using CommandHandler = std::function<CommandResult(const std::vector<std::string>&)>;
  void startConsoleLoop();
  void registerCommand(const std::string& name, CommandHandler handler);
  std::vector<std::string> parseLine(std::string line) const;
  std::string getInput() const;
private:
  std::unordered_map<std::string, CommandHandler> commands;
  
  // to be moved
  // void createGraph();
  // std::unique_ptr<graph::Graph<int>> getGraphFromFile(std::string path);
};
