#pragma once
#include <unordered_map>
#include <vector>
#include "../vertices/BaseVertex.hpp"
#include "views/AdjacentEdgesView.hpp"
#include <memory>

namespace graph {

using VertexSharedPtr = std::shared_ptr<BaseVertex>;

enum class GraphType {
  Directed,
  Undirected,
  Activity
};

class AdjacentEdgesView;

class Graph {
public:
  virtual ~Graph() = default;
  virtual GraphType getGraphType() const = 0;

  virtual void addVertex(const VertexSharedPtr &v) = 0;

  virtual void addEdge(const idT &fromId, const idT &toId, int weight = 1) = 0;
  virtual void removeVertex(const idT &id) = 0;
  virtual void removeEdge(const idT &fromId, const idT &toId) = 0;

  virtual bool isVertex(const idT &id) const = 0;
  virtual bool isEdge(const idT &fromId, const idT &toId) const = 0;

  virtual const VertexSharedPtr &getVertex(const idT &id) const = 0;
  virtual int getNrOfVertices() const = 0;
  virtual int getNrOfEdges() const = 0;
  virtual int getEdgeWeight(const idT &fromId, const idT &toId) const = 0;

  virtual std::vector<Edge> getEdges() const = 0;
  virtual void clear() = 0;

  virtual std::unordered_map<idT, VertexSharedPtr>::const_iterator begin() const = 0;
  virtual std::unordered_map<idT, VertexSharedPtr>::const_iterator end() const = 0;

  virtual AdjacentEdgesView getAdjacentEdges(const idT &id) const = 0;
};
} // namespace graph
