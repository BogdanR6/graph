#pragma once
#include "../abstract/Graph.hpp"
#include <unordered_map>

namespace graph {

// TODO: Modify the EdgeHash class such that for undirected the direction does not matter

class AdjacentEdgesView;

class UndirectedGraph : public Graph {
private:
  std::unordered_map<VertexSharedPtr, std::unordered_set<VertexSharedPtr>> adjacency;
  std::unordered_set<VertexSharedPtr> vertices;
  std::unordered_set<Edge, EdgeHash> edges;
  friend class AdjacentEdgesView;

public:
  GraphType getGraphType() const override;
  bool isVertex(const VertexSharedPtr &v) const override;
  bool isEdge(const VertexSharedPtr &from, const VertexSharedPtr &to) const override;
  void addVertex(const VertexSharedPtr &v) override;
  void removeVertex(const VertexSharedPtr &v) override;
  void addEdge(const VertexSharedPtr &from, const VertexSharedPtr &to, const int &weight = 1) override;
  void removeEdge(const VertexSharedPtr &from, const VertexSharedPtr &to) override;
  VertexSharedPtr &getVertex(const VertexSharedPtr &v) const override;
  int getNrOfVertices() const override;
  int getNrOfEdges() const override;
  int getEdgeWeight(const VertexSharedPtr &from, const VertexSharedPtr &to) const override;
  std::vector<Edge> getEdges() const override;
  void clear() override;
  std::unordered_set<VertexSharedPtr>::const_iterator begin() const override;
  std::unordered_set<VertexSharedPtr>::const_iterator end() const override;

  UndirectedGraph() {}
  UndirectedGraph(const UndirectedGraph &other) {
    this->adjacency = other.adjacency;
    this->vertices = other.vertices;
  }

  AdjacentEdgesView getAdjacentEdges(const VertexSharedPtr &v) const;

  UndirectedGraph &operator=(const UndirectedGraph &other) {
    if (this != &other) {
      this->adjacency = other.adjacency;
      this->vertices = other.vertices;
    }
    return *this;
  }
};

} //namespace graph
