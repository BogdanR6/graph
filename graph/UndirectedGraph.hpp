#pragma once
#include "Graph.hpp"
#include <stdexcept>
#include <unordered_map>

namespace graph {

// TODO: Modify the EdgeHash class such that for undirected the direction does not matter

template <typename T> class AdjacentEdgesView;

template <typename T> class UndirectedGraph : public Graph<T> {
private:
  std::unordered_map<T, std::unordered_set<T>> adjacency;
  std::unordered_set<T> vertices;
  std::unordered_set<Edge<T>, EdgeHash<T>> edges;
  friend class AdjacentEdgesView<T>;

public:
  GraphType getGraphType() const override;
  bool isVertex(const T &v) const override;
  bool isEdge(const T &from, const T &to) const override;
  void addVertex(const T &v) override;
  void removeVertex(const T &v) override;
  void addEdge(const T &from, const T &to, const int &weight = 1) override;
  void removeEdge(const T &from, const T &to) override;
  int getNrOfVertices() const override;
  int getNrOfEdges() const override;
  int getEdgeWeight(const T &from, const T &to) const override;
  std::vector<Edge<T>> getEdges() const override;
  void clear() override;
  std::unordered_set<T>::const_iterator begin() const override;
  std::unordered_set<T>::const_iterator end() const override;

  UndirectedGraph() {}
  UndirectedGraph(const UndirectedGraph<T> &other) {
    this->adjacency = other.adjacency;
    this->vertices = other.vertices;
  }

  AdjacentEdgesView<T> getAdjacentEdges(const T &v) const;

  UndirectedGraph<T> &operator=(const UndirectedGraph<T> &other) {
    if (this != &other) {
      this->adjacency = other.adjacency;
      this->vertices = other.vertices;
    }
    return *this;
  }
};


template <typename T>
std::vector<Edge<T>> UndirectedGraph<T>::getEdges() const {
  std::vector<Edge<T>> edgesV;
  for (const auto &edge : this->edges)
    edgesV.push_back(edge);
  return edgesV;
}


template <typename T>
int UndirectedGraph<T>::getEdgeWeight(const T &from, const T &to) const {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");

  // the edge can go either way because the graph is undirected
  if (!isEdge(from, to) && !isEdge(to, from))
    throw std::runtime_error("The edge does not exists");

  auto edge_it = edges.find(Edge<T>{from, to});
  if (edge_it == edges.end())
    edge_it = edges.find(Edge<T>{to, from});
  if (edge_it == edges.end())
    throw std::runtime_error("The edge " + from + " -- " + to + " does not exists in the edges set");

  return (*edge_it).weight;
}

template <typename T>
GraphType UndirectedGraph<T>::getGraphType() const {
  return GraphType::Undirected;
}

template <typename T> bool UndirectedGraph<T>::isVertex(const T &v) const {
  return adjacency.find(v) != adjacency.end();
}

template <typename T>
bool UndirectedGraph<T>::isEdge(const T &from, const T &to) const {
  return isVertex(from) && isVertex(to) &&
         adjacency.at(from).find(to) != adjacency.at(from).end() &&
         (edges.find({from, to}) != edges.end() || edges.find({to, from}) != edges.end());
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
void UndirectedGraph<T>::addEdge(const T &from, const T &to, const int &weight) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(from, to))
    throw std::runtime_error("The edge already exists");

  adjacency.at(from).insert(to);
  adjacency.at(to).insert(from);
  edges.insert(Edge<T>{from, to, weight});
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
AdjacentEdgesView<T> UndirectedGraph<T>::getAdjacentEdges(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex is not in the graph");
  return AdjacentEdgesView<T>(*this, v);
}

template <typename T>
class AdjacentEdgesView {
private:
    std::vector<Edge<T>> edges;

public:
    AdjacentEdgesView(const UndirectedGraph<T>& graph, const T& vertex) {
        // Find the vertex in the adjacency map
        auto it = graph.adjacency.find(vertex);
        if (it != graph.adjacency.end()) {
            // Populate edges vector
            for (const T& to : it->second) {
                edges.push_back(Edge<T>{vertex, to, graph.getEdgeWeight(vertex, to)});
            }
        }
    }

    // Standard container interface
    auto begin() const { return edges.begin(); }
    auto end() const { return edges.end(); }
    bool empty() const { return edges.empty(); }
};
} //namespace graph
