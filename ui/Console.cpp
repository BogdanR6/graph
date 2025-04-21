#include "Console.hpp"
#include "../errors/InvalidInputError.cpp"
#include <iostream>
#include <unordered_map>
#include <vector>

void Console::registerCommand(const std::string& name, CommandHandler handler) {
  commands[name] = handler;
}

void Console::documentCommand(const std::string& name, const std::string& description) {
  man[name] = description;
}

std::vector<std::string> Console::getRegisteredCommands() const {
  std::vector<std::string> commandsName;
  for (const auto &[name, _] : commands)
    commandsName.push_back(name);
  return commandsName;
}

std::unordered_map<std::string, std::string> Console::getMan() const {
  return man;
}

void Console::startConsoleLoop() {
  while (true) {
    std::vector<std::string> args = parseLine(getInput());
    if (args.empty())
      continue;

    auto commandMapIt = commands.find(args[0]);
    if (commandMapIt == commands.end()) {
      std::cout << "Unknown command: " << args[0] << "\n";
      continue;
    }

    CommandResult result;
    try {
      result = commandMapIt->second(args);
    } catch (InvalidUsageError& e) {
      std::cout << e.what() << '\n';
      continue;
    }
    
    if (!result.output.empty()) {
      std::cout << result.output << '\n';
    }
    if (result.shouldExit)
      return;
  }
}


std::string Console::getInput() const {
  std::string line;

  std::cout << "-> ";
  if (!getline(std::cin, line)) {
    std::cerr << "Error reading line. Exiting...\n";
    exit(1);
  }
  return line;
}


std::vector<std::string> Console::parseLine(std::string line) const {
  std::vector<std::string> parsedLine;
  std::string arg;
  bool inQuotes = false;
  char quoteChar = 0;

  for (size_t i = 0; i < line.length(); ++i) {
    char c = line[i];

    if (inQuotes) {
      if (c == quoteChar) {
        inQuotes = false;
      } else if (c == '\\' && i + 1 < line.length()) {
        // Handle escaped characters
        arg += line[++i];
      } else {
        arg += c;
      }
    } else {
      if (c == '"' || c == '\'') {
        inQuotes = true;
        quoteChar = c;
      } else if (isspace(c)) {
        if (!arg.empty()) {
          parsedLine.push_back(arg);
          arg.clear();
        }
      } else {
        arg += c;
      }
    }
  }

  if (!arg.empty()) {
    parsedLine.push_back(arg);
  }

  return parsedLine;
}


// std::unique_ptr<graph::Graph<int>> Console::getGraphFromFile(std::string path) {
//   std::unique_ptr<graph::Graph<int>> g;
//   if (graph->getGraphType() == graph::GraphType::Directed) {
//     g = std::make_unique<graph::DirectedGraph<int>>();
//   } else if (graph->getGraphType() == graph::GraphType::Undirected) {
//     g = std::make_unique<graph::UndirectedGraph<int>>();
//   }
//   std::ifstream fin(path);
//   int nrOfVertices, nrOfEdges;
//   fin >> nrOfVertices >> nrOfEdges;
//   for (int v : std::views::iota(0, nrOfVertices)) {
//     g->addVertex(v);
//   }
//   for (const auto &_ : std::views::iota(0, nrOfEdges)) {
//     int from, to, weight;
//     fin >> from >> to >> weight;
//     try {
//       g->addEdge(from, to, weight);
//     } catch (std::runtime_error &_) {
//       continue;
//     }
//   }
//   return g;
// }
// 
// 
// void Console::createGraph() {
//   return;
// }
