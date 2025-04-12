#pragma once
#include "../graph/Graph.hpp"
#include "../graph/UndirectedGraph.hpp"
#include "../graph/DirectedGraph.hpp"
#include <memory>
#include <string>

class GraphService {
public:
  GraphService(std::unique_ptr<graph::Graph<int>> graph) : graph(std::move(graph)) {}
  GraphService() : GraphService(std::make_unique<graph::UndirectedGraph<int>>()) {}

  void addVertex(const int &vertexId);
  void removeVertex(const int &vertexId);
  bool isVertex(const int &vertexId);
    
  void addEdge(const int &fromVertexId, const int &toVertexId, const int &weight);
  void removeEdge(const int &fromVertexId, const int &toVertexId);
  bool isEdge(const int &fromVertexId, const int &toVertexId);

  std::string getAdjacentVertices(const int &vertexId);
  std::string getOutboundVertices(const int &vertexId);

  std::string getVertices();

  std::string getEdges();

private:
  std::unique_ptr<graph::Graph<int>> graph;
};
