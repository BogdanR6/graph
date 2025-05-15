#pragma once
#include "../graph/Graph.hpp"
#include "../graph/UndirectedGraph.hpp"
#include <memory>
#include <string>
#include <vector>

using vertex_t = std::string;

class GraphService {
public:
  GraphService(std::unique_ptr<graph::Graph<vertex_t>> graph) : graph(std::move(graph)) {}
  GraphService() : GraphService(std::make_unique<graph::UndirectedGraph<vertex_t>>()) {}

  void addVertex(const vertex_t &vertexId);
  void removeVertex(const vertex_t &vertexId);
  bool isVertex(const vertex_t &vertexId);
    
  void addEdge(const vertex_t &fromVertexId, const vertex_t &toVertexId, const int &weight);
  void removeEdge(const vertex_t &fromVertexId, const vertex_t &toVertexId);
  bool isEdge(const vertex_t &fromVertexId, const vertex_t &toVertexId);

  std::string getAdjacentEdges(const vertex_t &vertexId) const;
  std::string getOutboundEdges(const vertex_t &vertexId) const;
  std::string getInboundEdges(const vertex_t &vertexId) const;

  std::string getVertices();

  std::string getEdges();

  std::string loadGraph(const std::string &path, const std::string &graphType);
  void saveGraph(const std::string& path) const;

  std::vector<graph::UndirectedGraph<vertex_t>> getConnectedComponentsOfUnorderedGraph() const;
  std::pair<std::vector<vertex_t>, int> getLowestCostWalk(const vertex_t &start, const vertex_t &end) const;
  std::vector<std::string> topologicalSort() const;

private:
  std::shared_ptr<graph::Graph<vertex_t>> graph;
};
