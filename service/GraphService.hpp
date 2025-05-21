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

  void addVertex(const BaseVertex &vertexId);
  void removeVertex(const BaseVertex &vertexId);
  bool isVertex(const BaseVertex &vertexId);
    
  void addEdge(const BaseVertex &fromVertexId, const BaseVertex &toVertexId, const int &weight);
  void removeEdge(const BaseVertex &fromVertexId, const BaseVertex &toVertexId);
  bool isEdge(const BaseVertex &fromVertexId, const BaseVertex &toVertexId);

  std::string getAdjacentEdges(const BaseVertex &vertexId) const;
  std::string getOutboundEdges(const BaseVertex &vertexId) const;
  std::string getInboundEdges(const BaseVertex &vertexId) const;

  std::string getVertices();

  std::string getEdges();

  std::string loadGraph(const std::string &path, const std::string &graphType);
  void saveGraph(const std::string& path) const;

  std::vector<graph::UndirectedGraph> getConnectedComponentsOfUnorderedGraph() const;
  std::pair<std::vector<BaseVertex>, int> getLowestCostWalk(const BaseVertex &start, const BaseVertex &end) const;
  std::vector<std::string> topologicalSort() const;

private:
  std::shared_ptr<graph::Graph> graph;
};
