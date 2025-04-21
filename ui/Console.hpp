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
  void registerCommand(const std::string &name, CommandHandler handler);
  void documentCommand(const std::string &name, const std::string &description);
  std::vector<std::string> getRegisteredCommands() const;
  std::unordered_map<std::string, std::string> getMan() const;
  std::vector<std::string> parseLine(std::string line) const;
  std::string getInput() const;
private:
  std::unordered_map<std::string, CommandHandler> commands;
  std::unordered_map<std::string, std::string> man;
};
