#include "GraphService.hpp"
#include "../errors/InvalidInputError.cpp"
#include "../graph/DirectedGraph.hpp"
#include "../graph/algorithms/UndirectedGraphAlgorithms.hpp"
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>


void GraphService::addVertex(const TElem& vertexId) {
  graph->addVertex(vertexId);
}


void GraphService::removeVertex(const TElem &vertexId) {
  graph->removeVertex(vertexId);
}


bool GraphService::isVertex(const TElem &vertexId) {
  return graph->isVertex(vertexId);
}
  

void GraphService::addEdge(const TElem &fromVertexId, const TElem &toVertexId, const int &weight) {
  graph->addEdge(fromVertexId, toVertexId, weight);
}


void GraphService::removeEdge(const TElem &fromVertexId, const TElem &toVertexId) {
  graph->removeEdge(fromVertexId, toVertexId);
}


bool GraphService::isEdge(const TElem &fromVertexId, const TElem &toVertexId) {
  return graph->isEdge(fromVertexId, toVertexId);
}


std::string GraphService::getVertices() {
  std::string vertices = "";
  for (const auto& vertex : *graph) {
    vertices += vertex + " ";
  }
  return vertices; 
}


std::string GraphService::getAdjacentEdges(const TElem &vertexId) const {
  // TODO: add support for all types of graphs 
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw InvalidOperationOnGraphType("Adjacent Vertices are available only for Undirected graphs");
  std::string vertices = "The adjacent vertices of " + vertexId + " are:\n";
  auto *undirected = dynamic_cast<graph::UndirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& [_, vertex, __] : undirected->getAdjacentEdges(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no adjacent vertices";
  }
  return vertices; 
}


std::string GraphService::getOutboundEdges(const TElem &vertexId) const {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Outbound Vertices are defined only for Directed graphs");
  std::string vertices = "The outbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& [_, vertex, __] : directed->getOutboundEdges(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no outbound vertices";
  }
  return vertices;
}


std::string GraphService::getInboundEdges(const TElem &vertexId) const {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Inbound Vertices are defined only for Directed graphs");
  std::string vertices = "The inbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& [vertex, _, __] : directed->getInboundEdges(vertexId)) {
    vertices += vertex + " "; 
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no inbound vertices";
  }
  return vertices;
}


std::string GraphService::getEdges() {
  std::string delimitor = "--";
  if (graph->getGraphType() == graph::GraphType::Directed)
    delimitor = "->";
  std::string output = "The edges in the graph are:\n";
  for (const auto& edge : graph->getEdges()) {
    output += edge.from + delimitor + edge.to + "\n";
  }
  output = output.substr(0, output.length() - 1); // eliminate the last new line
  return output;
}

std::string GraphService::loadGraph(const std::string &path) {
  std::ifstream fin(path);
  if (!fin.is_open())
    throw std::runtime_error("Could not open file '" + path + "' for reading");

  graph->clear();

  auto split = [](const std::string &str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
      tokens.push_back(token);
    return tokens;
  };

  std::string line;
  if (!std::getline(fin, line))
    throw std::runtime_error("Empty file");

  std::vector<std::string> firstLine = split(line);

  if (firstLine.size() == 2 &&
    std::all_of(firstLine[0].begin(), firstLine[0].end(), ::isdigit) &&
    std::all_of(firstLine[1].begin(), firstLine[1].end(), ::isdigit)) {
    // Format 1: vertex_count edge_count
    int vertexCount = std::stoi(firstLine[0]);
    for (int i = 0; i < vertexCount; ++i)
        graph->addVertex(std::to_string(i));
      
    int edgeCount = std::stoi(firstLine[1]);

    for (int i = 0; i < edgeCount; ++i) {
      if (!std::getline(fin, line))
        throw std::runtime_error("Unexpected end of file while reading edges");
      auto tokens = split(line);
      if (tokens.size() != 3)
        throw std::runtime_error("Expected format 'from to cost' on line " + std::to_string(i + 2));

      std::string from = tokens[0];
      std::string to = tokens[1];
      int cost = std::stoi(tokens[2]);

      try {
        graph->addVertex(from);
      } catch (...){} // ignore if already exists
      try {
        graph->addVertex(to);
      } catch (...){} // ignore if already exists
      try {
        graph->addEdge(from, to, cost);
      } catch (...){} // ignore if already exists
    }
  } else {
    bool hasCost = firstLine.size() == 3;

    do {
      auto tokens = firstLine;  // the split line 

      if (tokens.size() == 1) {
        // Single vertex: isolated vertex
        std::string vertex = tokens[0];
        try {
          graph->addVertex(vertex);
        } catch (...) {} // ignore if already exists
      } 
      else if (tokens.size() == 2 || tokens.size() == 3) {
        // Edge: from to [cost]
        std::string from = tokens[0];
        std::string to = tokens[1];
        int cost = 1;
        if (tokens.size() == 3)
          cost = std::stoi(tokens[2]);

        try {
            graph->addVertex(from);
        } catch (...) {}
        try {
            graph->addVertex(to);
        } catch (...) {}
        try {
            graph->addEdge(from, to, cost);
        } catch (...) {}
      } 
      else {
          throw std::runtime_error("Invalid line format: '" + line + "'");
      }

    } while (std::getline(fin, line) && (firstLine = split(line), !line.empty()));
}

  return "Successfully loaded the graph";
}


void GraphService::saveGraph(const std::string& path) const {
  std::ofstream fout(path);
  if (!fout.is_open())
    throw std::runtime_error("Could not open file '" + path + "' for writing");

  if (graph->getGraphType() == graph::GraphType::Directed) {
    auto *directed = dynamic_cast<graph::DirectedGraph<TElem>*>(graph.get());
    for (const auto &from : *directed) {
      for (const auto &[_, to, cost] : directed->getOutboundEdges(from))
          fout << from << " " << to << " " << cost << "\n";
    }
    for (const auto& vertex : *directed) {
      auto outEdges = directed->getOutboundEdges(vertex);
      auto inEdges = directed->getInboundEdges(vertex);

      if (outEdges.begin() == outEdges.end() && inEdges.begin() == inEdges.end()) {
        fout << vertex << "\n";
      }
    }
  }

  else if (graph->getGraphType() == graph::GraphType::Undirected) {
    auto *undirected = dynamic_cast<graph::UndirectedGraph<TElem>*>(graph.get());
    for (const auto &vertex : *undirected) {
      auto adjacentEdges = undirected->getAdjacentEdges(vertex);
      if (adjacentEdges.empty())
        fout << vertex << "\n";
      else for (const auto &edge : adjacentEdges) {
          fout << edge.from << " " << edge.to << " " << edge.weight << "\n";
      }
    }
  }
  fout.close();
}


std::vector<graph::UndirectedGraph<TElem>> GraphService::getConnectedComponentsOfUnorderedGraph() const {
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw std::runtime_error("getConnectedComponentsOfUndirectedGraph is only available for undirected graphs");
  auto undirected = dynamic_cast<graph::UndirectedGraph<TElem>*>(graph.get());
  return graph::algorithms::getConnectedComponentsDFS(*undirected);
}
