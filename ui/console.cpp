#include "console.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <ranges>

void Console::startConsoleLoop() {
  if (graph->getGraphType() == graph::GraphType::Directed) {
      std::cout << "It's a DirectedGraph!\n";
  } else if (graph->getGraphType() == graph::GraphType::Undirected) {
      std::cout << "It's an UndirectedGraph!\n";
  } else {
      std::cout << "Unknown graph type.\n";
  }
}

std::unique_ptr<graph::Graph<int>> Console::getGraphFromFile(std::string path) {
  std::unique_ptr<graph::Graph<int>> g;
  if (graph->getGraphType() == graph::GraphType::Directed) {
    g = std::make_unique<graph::DirectedGraph<int>>();
  } else if (graph->getGraphType() == graph::GraphType::Undirected) {
    g = std::make_unique<graph::UndirectedGraph<int>>();
  }
  std::ifstream fin(path);
  int nrOfVertices, nrOfEdges;
  fin >> nrOfVertices >> nrOfEdges;
  for (int v : std::views::iota(0, nrOfVertices)) {
    g->addVertex(v);
  }
  for (const auto &_ : std::views::iota(0, nrOfEdges)) {
    int from, to, weight;
    fin >> from >> to >> weight;
    try {
      g->addEdge(from, to, weight);
    } catch (std::runtime_error &_) {
      continue;
    }
  }
  return g;
}

void Console::createGraph() {
  return;
}
