#pragma once
#include "graph.hpp"
#include <stdexcept>
#include <unordered_map>

namespace graph {

template <typename T> class AdjacentVerticesIterator;

template <typename T> class UndirectedGraph : public Graph<T> {
private:
  std::unordered_map<T, std::unordered_set<T>> adjacency;
  std::unordered_set<T> vertices;
  friend class AdjacentVerticesIterator<T>;

public:
  bool isVertex(const T &v) const override;
  bool isEdge(const T &from, const T &to) const override;
  void addVertex(const T &v) override;
  void removeVertex(const T &v) override;
  void addEdge(const T &from, const T &to) override;
  void removeEdge(const T &from, const T &to) override;
  int getNrOfVertices() const override;
  int getNrOfEdges() const override;
  void clear() override;
  std::unordered_set<T>::const_iterator begin() const override;
  std::unordered_set<T>::const_iterator end() const override;

  UndirectedGraph() {}
  UndirectedGraph(const UndirectedGraph<T> &other) {
    this->adjacency = other.adjacency;
    this->vertices = other.vertices;
  }

  AdjacentVerticesIterator<T> getAdjacentVertices(const T &v) const;

  UndirectedGraph<T> &operator=(const UndirectedGraph<T> &other) {
    if (this != &other) {
      this->adjacency = other.adjacency;
      this->vertices = other.vertices;
    }
    return *this;
  }
};

template <typename T> bool UndirectedGraph<T>::isVertex(const T &v) const {
  return adjacency.find(v) != adjacency.end();
}

template <typename T>
bool UndirectedGraph<T>::isEdge(const T &from, const T &to) const {
  return isVertex(from) && isVertex(to) &&
         adjacency.at(from).find(to) != adjacency.at(from).end();
}

template <typename T> void UndirectedGraph<T>::addVertex(const T &v) {
  if (isVertex(v))
    throw std::runtime_error("Vertex Already added");

  adjacency[v] = std::unordered_set<T>();
  vertices.insert(v);
}

template <typename T> void UndirectedGraph<T>::removeVertex(const T &v) {
  if (!isVertex(v))
    throw std::runtime_error("Vertex not in the graph");

  for (const auto &ver : adjacency[v]) {
    adjacency[ver].erase(v);
  }
  adjacency.erase(v);
  vertices.erase(v);
}

template <typename T>
void UndirectedGraph<T>::addEdge(const T &from, const T &to) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(from, to))
    throw std::runtime_error("The edge already exists");

  adjacency[from].insert(to);
  adjacency[to].insert(from);
}

template <typename T>
void UndirectedGraph<T>::removeEdge(const T &from, const T &to) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(from, to))
    throw std::runtime_error("The edge does not exist");

  adjacency[from].erase(to);
  adjacency[to].erase(from);
}

template <typename T> int UndirectedGraph<T>::getNrOfVertices() const {
  return vertices.size();
}

template <typename T> int UndirectedGraph<T>::getNrOfEdges() const {
  int count = 0;
  for (const auto &[_, neighbors] : adjacency) {
    count += neighbors.size();
  }
  return count / 2;
}

template <typename T> void UndirectedGraph<T>::clear() {
  adjacency.clear();
  vertices.clear();
}

template <typename T>
std::unordered_set<T>::const_iterator UndirectedGraph<T>::begin() const {
  return vertices.begin();
}

template <typename T>
std::unordered_set<T>::const_iterator UndirectedGraph<T>::end() const {
  return vertices.end();
}

template <typename T>
AdjacentVerticesIterator<T>
UndirectedGraph<T>::getAdjacentVertices(const T &v) const {
  return AdjacentVerticesIterator<T>(*this, v);
}

template <typename T> class AdjacentVerticesIterator {
private:
  const UndirectedGraph<T> &graph;
  const T &vertex;

public:
  AdjacentVerticesIterator(const UndirectedGraph<T> &graph, const T &v)
      : graph(graph), vertex(v) {}
  std::unordered_set<T>::const_iterator begin() const {
    return graph.adjacency.at(vertex).begin();
  }
  std::unordered_set<T>::const_iterator end() const {
    return graph.adjacency.at(vertex).end();
  }
};
} // namespace graph
