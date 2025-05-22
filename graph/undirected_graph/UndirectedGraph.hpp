#pragma once
#include "../abstract/Graph.hpp"
#include "../abstract/views/AdjacentEdgesView.hpp"
#include <unordered_map>
#include <unordered_set>

namespace graph {

// TODO: Modify the EdgeHash class such that for undirected the direction does not matter

class UndirectedGraph : public Graph {
private:
  std::unordered_map<idT, std::unordered_set<idT>> adjacency;
  std::unordered_map<idT, VertexSharedPtr> vertices;
  std::unordered_set<Edge, EdgeHash> edges;

public:
  GraphType getGraphType() const override;

  bool isVertex(const idT &id) const override;
  void addVertex(const VertexSharedPtr &v) override;
  void removeVertex(const idT &id) override;
  const VertexSharedPtr &getVertex(const idT &id) const override;
  int getNrOfVertices() const override;

  bool isEdge(const idT &fromId, const idT &toId) const override;
  void addEdge(const idT &fromId, const idT &toId, int weight = 1) override;
  void removeEdge(const idT &fromId, const idT &toId) override;
  int getNrOfEdges() const override;
  int getEdgeWeight(const idT &fromId, const idT &to) const override;
  std::vector<Edge> getEdges() const override;

  void clear() override;
  std::unordered_map<idT, VertexSharedPtr>::const_iterator begin() const override;
  std::unordered_map<idT, VertexSharedPtr>::const_iterator end() const override;

  UndirectedGraph() {}
  UndirectedGraph(const UndirectedGraph &other) {
    this->adjacency = other.adjacency;
    this->vertices = other.vertices;
  }

  AdjacentEdgesView getAdjacentEdges(const idT &id) const override;

  UndirectedGraph &operator=(const UndirectedGraph &other) {
    if (this != &other) {
      this->adjacency = other.adjacency;
      this->vertices = other.vertices;
    }
    return *this;
  }
};

} //namespace graph
