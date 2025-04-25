#pragma once
#include "../graph/Graph.hpp"
#include "../graph/UndirectedGraph.hpp"
#include <memory>
#include <string>

using TElem = std::string;

class GraphService {
public:
  GraphService(std::unique_ptr<graph::Graph<TElem>> graph) : graph(std::move(graph)) {}
  GraphService() : GraphService(std::make_unique<graph::UndirectedGraph<TElem>>()) {}

  void addVertex(const TElem &vertexId);
  void removeVertex(const TElem &vertexId);
  bool isVertex(const TElem &vertexId);
    
  void addEdge(const TElem &fromVertexId, const TElem &toVertexId, const int &weight);
  void removeEdge(const TElem &fromVertexId, const TElem &toVertexId);
  bool isEdge(const TElem &fromVertexId, const TElem &toVertexId);

  std::string getAdjacentVertices(const TElem &vertexId);
  std::string getOutboundVertices(const TElem &vertexId);
  std::string getOutboundInbound(const TElem &vertexId);

  std::string getVertices();

  std::string getEdges();

  std::string loadGraph(const std::string &path);

private:
  std::unique_ptr<graph::Graph<TElem>> graph;
};
