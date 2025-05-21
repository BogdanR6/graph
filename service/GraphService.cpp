#include "GraphService.hpp"
#include "../graph/vertices/StringVertex.hpp"
#include "../errors/InvalidInputError.cpp"
#include "../graph/directed_graph/DirectedGraph.hpp"
#include "../graph/algorithms/UndirectedGraphAlgorithms.hpp"
#include "../graph/algorithms/DirectedGraphAlgorithms.hpp"
#include "ActivityGraph.hpp"
#include "Graph.hpp"
#include "StringVertex.hpp"
#include "UndirectedGraph.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>


void GraphService::addVertex(const BaseVertex& vertex) {
  graph->addVertex(std::make_shared<BaseVertex>(vertex));
}


void GraphService::removeVertex(const idT &vertexId) {
  graph->removeVertex(vertexId);
}


bool GraphService::isVertex(const idT &vertexId) {
  return graph->isVertex(vertexId);
}
  

void GraphService::addEdge(const idT &fromVertexId, const idT &toVertexId, int weight) {
  graph->addEdge(fromVertexId, toVertexId, weight);
}


void GraphService::removeEdge(const idT &fromVertexId, const idT &toVertexId) {
  graph->removeEdge(fromVertexId, toVertexId);
}


bool GraphService::isEdge(const idT &fromVertexId, const idT &toVertexId) {
  return graph->isEdge(fromVertexId, toVertexId);
}


std::string GraphService::getVertices() { // TODO: change it to return a std::vector<idT>
  std::string vertices = "";
  for (const auto& [vertexId, _] : *graph) {
    vertices += vertexId + " ";
  }
  return vertices; 
}


std::string GraphService::getAdjacentEdges(const idT &vertexId) const { // TODO: change it to return a std::vector<Edge>
  // TODO: add support for all types of graphs 
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw InvalidOperationOnGraphType("Adjacent Vertices are available only for Undirected graphs");
  std::string vertices = "The adjacent vertices of " + vertexId + " are:\n";
  auto *undirected = dynamic_cast<graph::UndirectedGraph*>(graph.get());
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


std::string GraphService::getOutboundEdges(const idT &vertexId) const {// TODO: change it to return a std::vector<Edge>
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Outbound Vertices are defined only for Directed graphs");
  std::string vertices = "The outbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  int count = 0;
  for (const auto& [_, vertex, __] : directed->initOutboundEdgesIt(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no outbound vertices";
  }
  return vertices;
}


std::string GraphService::getInboundEdges(const idT &vertexId) const { // TODO: change it to return a std::vector<Edge>
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Inbound Vertices are defined only for Directed graphs");
  std::string vertices = "The inbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  int count = 0;
  for (const auto& [vertex, _, __] : directed->initInboundEdgesIt(vertexId)) {
    vertices += vertex + " "; 
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no inbound vertices";
  }
  return vertices;
}


std::string GraphService::getEdges() { // TODO: make it return std::vector<Edge>
  std::string delimitor = "--";
  if (graph->getGraphType() == graph::GraphType::Directed)
    delimitor = "->";
  std::string output = "The edges in the graph are:\n";
  for (const auto& [fromId, toId, _] : graph->getEdges()) {
    output += fromId + delimitor + toId + "\n";
  }
  output = output.substr(0, output.length() - 1); // eliminate the last new line
  return output;
}

std::string GraphService::loadGraph(const std::string &path, const std::string &graphType) {
  std::ifstream fin(path);
  if (!fin.is_open())
    throw std::runtime_error("Could not open file '" + path + "' for reading");
  if (graphType == "undirected")
    graph = std::make_shared<graph::UndirectedGraph>();
  else if (graphType == "directed") 
    graph = std::make_shared<graph::DirectedGraph>();
  else 
    throw std::runtime_error("'" + graphType + "' is not a valid graph type");
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
        graph->addVertex(std::make_shared<StringVertex>(std::to_string(i)));
      
    int edgeCount = std::stoi(firstLine[1]);

    for (int i = 0; i < edgeCount; ++i) {
      if (!std::getline(fin, line))
        throw std::runtime_error("Unexpected end of file while reading edges");
      auto tokens = split(line);
      if (tokens.size() != 3)
        throw std::runtime_error("Expected format 'from to cost' on line " + std::to_string(i + 2));

      idT fromId = tokens[0];
      idT toId = tokens[1];
      int cost = std::stoi(tokens[2]);

      try {
        graph->addVertex(std::make_shared<StringVertex>(fromId));
      } catch (...){} // ignore if already exists
      try {
        graph->addVertex(std::make_shared<StringVertex>(toId));
      } catch (...){} // ignore if already exists
      try {
        graph->addEdge(fromId, toId, cost);
      } catch (...){} // ignore if already exists
    }
  } else {
    bool hasCost = firstLine.size() == 3;

    do {
      auto tokens = firstLine;  // the split line 

      if (tokens.size() == 1) {
        // Single vertex: isolated vertex
        idT vertexId = tokens[0];
        try {
        graph->addVertex(std::make_shared<StringVertex>(vertexId));
        } catch (...) {} // ignore if already exists
      } 
      else if (tokens.size() == 2 || tokens.size() == 3) {
        // Edge: from to [cost]
        idT fromId = tokens[0];
        idT toId = tokens[1];
        int cost = 1;
        if (tokens.size() == 3)
          cost = std::stoi(tokens[2]);

        try {
            graph->addVertex(std::make_shared<StringVertex>(fromId));
        } catch (...) {}
        try {
            graph->addVertex(std::make_shared<StringVertex>(toId));
        } catch (...) {}
        try {
            graph->addEdge(fromId, toId, cost);
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
    auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
    for (const auto &from : *directed) {
      for (const auto &[_, to, cost] : directed->initOutboundEdgesIt(from))
          fout << from << " " << to << " " << cost << "\n";
    }
    for (const auto& vertex : *directed) {
      auto outEdges = directed->initOutboundEdgesIt(vertex);
      auto inEdges = directed->initInboundEdgesIt(vertex);

      if (outEdges.begin() == outEdges.end() && inEdges.begin() == inEdges.end()) {
        fout << vertex << "\n";
      }
    }
  }

  else if (graph->getGraphType() == graph::GraphType::Undirected) {
    auto *undirected = dynamic_cast<graph::UndirectedGraph*>(graph.get());
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


std::vector<graph::UndirectedGraph> GraphService::getConnectedComponentsOfUnorderedGraph() const {
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw std::runtime_error("getConnectedComponentsOfUndirectedGraph is only available for undirected graphs");
  auto undirected = dynamic_cast<graph::UndirectedGraph*>(graph.get());
  return graph::algorithms::getConnectedComponentsDFS(*undirected);
}


std::pair<std::vector<idT>, int> GraphService::getLowestCostWalk(const BaseVertex &start, const BaseVertex &end) const {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw std::runtime_error("getLowestCostWalk is only available for directed graphs");
  auto directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  return graph::algorithms::getLowestCostWalk(*directed, start, end);
}


std::vector<idT> GraphService::topologicalSort() const {
  if (graph->getGraphType() != graph::GraphType::Directed) 
    throw std::runtime_error("topologicalSort is only available for directed graphs");
  auto directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  return graph::algorithms::getTopologicalOrder(*directed);
}
