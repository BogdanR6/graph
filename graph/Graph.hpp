#pragma once
#include <unordered_set>
#include <vector>

namespace graph {

enum class GraphType {
  Directed,
  Undirected,
  Activity
};

template <typename T>
struct Edge {
  T from;
  T to;
  int weight;
  Edge(const T &from, const T &to) : from(from), to(to), weight(1) {}
  Edge(const T &from, const T &to, const int &weight) : from(from), to(to), weight(weight) {}
  bool operator==(const Edge& other) const {
    return from == other.from && to == other.to;
  }
};

template <typename T>
struct EdgeHash {
  std::size_t operator()(const Edge<T>& e) const {
    return std::hash<T>()(e.from) ^ (std::hash<T>()(e.to) << 1);
  }
};

template <typename T> class Graph {
public:
  virtual ~Graph() = default;
  virtual GraphType getGraphType() const = 0;

  virtual void addVertex(const T &v) = 0;
  virtual void addEdge(const T &from, const T &to, const int& weight = 1) = 0;
  virtual void removeVertex(const T &v) = 0;
  virtual void removeEdge(const T &from, const T &to) = 0;
  virtual bool isVertex(const T &v) const = 0;
  virtual bool isEdge(const T &from, const T &to) const = 0;
  virtual T &getVertex(const T &v) const = 0;
  virtual int getNrOfVertices() const = 0;
  virtual int getNrOfEdges() const = 0;
  virtual int getEdgeWeight(const T &from, const T &to) const = 0;
  virtual std::vector<Edge<T>> getEdges() const = 0;
  virtual void clear() = 0;
  // include a get adjacent vertices function

  virtual std::unordered_set<T>::const_iterator begin() const = 0;
  virtual std::unordered_set<T>::const_iterator end() const = 0;
};
} // namespace graph
