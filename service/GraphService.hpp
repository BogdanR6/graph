#pragma once
#include "../graph/abstract/Graph.hpp"
#include "../graph/undirected_graph/UndirectedGraph.hpp"
#include "../graph/vertices/BaseVertex.hpp"
#include <memory>
#include <string>
#include <vector>

class GraphService {
public:
  GraphService(std::unique_ptr<graph::Graph> graph) : graph(std::move(graph)) {}
  GraphService() : GraphService(std::make_unique<graph::UndirectedGraph>()) {}

  void addVertex(const BaseVertex &vertex);
  void removeVertex(const idT &vertexId);
  bool isVertex(const idT &vertexId);
    
  void addEdge(const idT &fromVertexId, const idT &toVertexId, int weight = 1);
  void removeEdge(const idT &fromVertexId, const idT &toVertexId);
  bool isEdge(const idT &fromVertexId, const idT &toVertexId);

  std::string getAdjacentEdges(const idT &vertexId) const;
  std::string getOutboundEdges(const idT &vertexId) const;
  std::string getInboundEdges(const idT &vertexId) const;

  std::string getVertices();

  std::string getEdges();

  std::string loadGraph(const std::string &path, const std::string &graphType);
  void saveGraph(const std::string& path) const;

  std::vector<graph::UndirectedGraph> getConnectedComponentsOfUnorderedGraph() const;
  std::pair<std::vector<idT>, int> getLowestCostWalk(const BaseVertex &start, const BaseVertex &end) const;
  std::vector<idT> topologicalSort() const;

private:
  std::shared_ptr<graph::Graph> graph;
};
