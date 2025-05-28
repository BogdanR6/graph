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

  graph::GraphType getGraphType() const;

  void addVertex(const graph::VertexSharedPtr &vertex);
  void removeVertex(const graph::idT &vertexId);
  bool isVertex(const graph::idT &vertexId);
    
  void addEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId, int weight = 1);
  void removeEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId);
  bool isEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId);

  std::vector<graph::Edge> getAdjacentEdges(const graph::idT &vertexId) const;
  std::vector<graph::Edge> getOutboundEdges(const graph::idT &vertexId) const;
  std::vector<graph::Edge> getInboundEdges(const graph::idT &vertexId) const;

  std::vector<graph::VertexSharedPtr> getVertices();

  std::vector<graph::Edge> getEdges();

  void loadGraph(const std::string &path, const std::string &graphType);
  void saveGraph(const std::string& path) const;

  std::vector<graph::UndirectedGraph> getConnectedComponentsOfUnorderedGraph() const;
  std::pair<std::vector<graph::idT>, int> getLowestCostWalk(const graph::idT &startId, const graph::idT &endId) const;
  std::vector<graph::idT> topologicalSort() const;

  // for activity graph
  int getTotalProjectTime();
  std::vector<graph::idT> getCriticalActivities();

  // for lab 5
  std::vector<graph::idT> getMinimumVertexCover();

private:
  std::shared_ptr<graph::Graph> graph;
};
