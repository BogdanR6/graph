#pragma once
#include "../abstract/Graph.hpp"
#include <unordered_map>
#include <unordered_set>

namespace graph {

class InboundEdgesIterator;

class OutboundEdgesIterator;

class DirectedGraph : public Graph {
private:
  std::unordered_set<Edge, EdgeHash> weights;
  std::unordered_map<idT, VertexSharedPtr> vertices;
  std::unordered_map<idT, std::unordered_set<idT>> outAdjacency;
  std::unordered_map<idT, std::unordered_set<idT>> inAdjacency;
  friend class InboundEdgesIterator;
  friend class OutboundEdgesIterator;

public:
  DirectedGraph() : inAdjacency(), outAdjacency(), weights(), vertices() {}

  GraphType getGraphType() const override;
  // Methods on vertices
  bool isVertex(const idT &id) const override;
  void addVertex(const VertexSharedPtr &v) override;
  void removeVertex(const idT &id) override;
  const VertexSharedPtr &getVertex(const idT &id) const override;
  int getNrOfVertices() const override;
  std::unordered_set<idT> getAllOutboundVertices(const idT &id) const;
  std::unordered_set<idT> getAllInboundVertices(const idT &id) const;

  // Methods on edges
  bool isEdge(const idT &fromId, const idT &toId) const override;
  void addEdge(const idT &fromId, const idT &toId, int weight = 1) override;
  void removeEdge(const idT &fromId, const idT &toId) override;
  int getNrOfEdges() const override;
  int getEdgeWeight(const idT &fromId, const idT &toId) const override;

  std::vector<Edge> getEdges() const override;



  // Iterators
  std::unordered_map<idT, VertexSharedPtr>::const_iterator begin() const override;
  std::unordered_map<idT, VertexSharedPtr>::const_iterator end() const override;

  OutboundEdgesIterator initOutboundEdgesIt(const idT &id) const;
  InboundEdgesIterator initInboundEdgesIt(const idT &id) const;


  // Misc Methods
  void clear() override;

  int getInDegree(const idT &id) const;

  int getOutDegree(const idT &id) const;
};
}// namespace graph
