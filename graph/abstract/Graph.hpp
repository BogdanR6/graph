#pragma once
#include <unordered_set>
#include <vector>
#include "../vertices/BaseVertex.hpp"
#include <memory>

using VertexSharedPtr = std::shared_ptr<BaseVertex>;

namespace graph {

enum class GraphType {
  Directed,
  Undirected,
  Activity
};

struct Edge {
  std::string fromId;
  std::string toId;
  int weight;

  Edge(const std::string &from, const std::string &to, int weight = 1)
      : fromId(from), toId(to), weight(weight) {}

  bool operator==(const Edge &other) const {
    return fromId == other.fromId && toId == other.toId;
  }
};

struct EdgeHash {
  std::size_t operator()(const Edge &e) const {
    return std::hash<std::string>()(e.fromId) ^ (std::hash<std::string>()(e.toId) << 1);
  }
};

class Graph {
public:
  virtual ~Graph() = default;
  virtual GraphType getGraphType() const = 0;

  virtual void addVertex(const VertexSharedPtr &v) = 0;

  virtual void addEdge(const std::string &fromId, const std::string &toId, int weight = 1) = 0;
  virtual void removeVertex(const std::string &id) = 0;
  virtual void removeEdge(const std::string &fromId, const std::string &toId) = 0;

  virtual bool isVertex(const std::string &id) const = 0;
  virtual bool isEdge(const std::string &fromId, const std::string &toId) const = 0;

  virtual VertexSharedPtr getVertex(const std::string &id) const = 0;
  virtual int getNrOfVertices() const = 0;
  virtual int getNrOfEdges() const = 0;
  virtual int getEdgeWeight(const std::string &fromId, const std::string &toId) const = 0;

  virtual std::vector<Edge> getEdges() const = 0;
  virtual void clear() = 0;

  //virtual std::unordered_map<std::string, VertexSharedPtr>::const_iterator begin() const = 0;
  //virtual std::unordered_map<std::string, VertexSharedPtr>::const_iterator end() const = 0;
  virtual std::unordered_set<VertexSharedPtr>::const_iterator begin() const = 0;
  virtual std::unordered_set<VertexSharedPtr>::const_iterator end() const = 0;
};
} // namespace graph
