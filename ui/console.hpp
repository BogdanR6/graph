#pragma once
#include "../graph/graph.hpp"
#include "../graph/directedGraph.hpp"
#include "../graph/undirectedGraph.hpp"
#include <memory>

#if defined(__linux__) // Or #if __linux__
#define CLEAR_SCREEN system("clear");
#elif _WIN32
#define CLEAR_SCREEN system("cls");
#else
#define CLEAR_SCREEN system("clear");
#endif

class Console {
private:
  std::unique_ptr<graph::Graph<int>> graph; 

  void createGraph();
  std::unique_ptr<graph::Graph<int>> getGraphFromFile(std::string path);
public:
  Console(std::unique_ptr<graph::Graph<int>> _graph) : graph(std::move(_graph)) {}
  Console() : Console(std::make_unique<graph::UndirectedGraph<int>>()) {}
  void startConsoleLoop();
};
