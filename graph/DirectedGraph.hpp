#pragma once
#include "Graph.hpp"
#include <stdexcept>
#include <unordered_map>

namespace graph {

template <typename T> class InboundEdgesIterator;

template <typename T> class OutboundEdgesIterator;

template <typename T> class DirectedGraph : public Graph<T> {
private:
  std::unordered_map<T, std::unordered_set<T>> inAdjacency;
  std::unordered_map<T, std::unordered_set<T>> outAdjacency;
  std::unordered_map<Edge<T>, int, EdgeHash<T>> weights;
  std::unordered_set<T> vertices;
  friend class InboundEdgesIterator<T>;
  friend class OutboundEdgesIterator<T>;

public:
  GraphType getGraphType() const override;
  bool isVertex(const T &v) const override;
  bool isEdge(const T &from, const T &to) const override;
  void addVertex(const T &v) override;
  void removeVertex(const T &v) override;
  void addEdge(const T &from, const T &to, const int &weight = 1) override;
  void removeEdge(const T &from, const T &to) override;
  T &getVertex(const T &v) const override;
  int getNrOfVertices() const override;
  int getNrOfEdges() const override;
  int getEdgeWeight(const T &from, const T &to) const override;
  std::vector<Edge<T>> getEdges() const override;
  void clear() override;
  std::unordered_set<T>::const_iterator begin() const override;
  std::unordered_set<T>::const_iterator end() const override;

  DirectedGraph() : inAdjacency(), outAdjacency(), weights(), vertices() {}
  int getInDegree(const T &v) const;
  int getOutDegree(const T &v) const;

  OutboundEdgesIterator<T> initOutboundEdgesIt(const T &v) const;
  InboundEdgesIterator<T> initInboundEdgesIt(const T &v) const;

  std::unordered_set<T> getAllOutboundVertices(const T &v) const;
  std::unordered_set<T> getAllInnoundVertices(const T &v) const;
};

template <typename T>
std::unordered_set<T> DirectedGraph<T>::getAllInnoundVertices(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("v is not in the graph");

  return inAdjacency.at(v);
}

template <typename T>
std::unordered_set<T> DirectedGraph<T>::getAllOutboundVertices(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("v is not in the graph");

  return outAdjacency.at(v);
}

template <typename T>
std::vector<Edge<T>> DirectedGraph<T>::getEdges() const {
  std::vector<Edge<T>> edges;
  for (const auto &[edge, _] : weights)
    edges.push_back(edge);
  return edges;
}

template <typename T>
GraphType DirectedGraph<T>::getGraphType() const {
  return GraphType::Directed;
}

template <typename T>
int DirectedGraph<T>::getNrOfVertices() const {
  return vertices.size();
}


template <typename T>
int DirectedGraph<T>::getNrOfEdges() const {
  return weights.size();
}


template <typename T>
void DirectedGraph<T>::clear() {
  inAdjacency.clear();
  outAdjacency.clear();
  vertices.clear();
  weights.clear();
}


template <typename T> 
int DirectedGraph<T>::getEdgeWeight(const T &from, const T &to) const {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(from, to))
    throw std::runtime_error("The edge does not exit exists");
  return weights.at({from, to});
}

template <typename T> bool DirectedGraph<T>::isVertex(const T &v) const {
  return vertices.find(v) != vertices.end();
}

template <typename T>
bool DirectedGraph<T>::isEdge(const T &from, const T &to) const {
  return isVertex(from) && isVertex(to) &&
         outAdjacency.at(from).find(to) != outAdjacency.at(from).end();
}

template <typename T> void DirectedGraph<T>::addVertex(const T &v) {
  if (isVertex(v))
    throw std::runtime_error("Vertex Already added");

  outAdjacency[v] = std::unordered_set<T>();
  inAdjacency[v] = std::unordered_set<T>();
  vertices.insert(v);
}

template <typename T> void DirectedGraph<T>::removeVertex(const T &v) {
  if (!isVertex(v))
    throw std::runtime_error("Vertex not in the graph");

  for (const auto &ver : outAdjacency[v]) {
    inAdjacency[ver].erase(v);
  }
  outAdjacency.erase(v);
  vertices.erase(v);
}

template <typename T>
void DirectedGraph<T>::addEdge(const T &from, const T &to, const int &weight) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(from, to))
    throw std::runtime_error("The edge already exists");

  outAdjacency[from].insert(to);
  inAdjacency[to].insert(from);
  weights[{from, to}] = weight;
}

template <typename T>
T &DirectedGraph<T>::getVertex(const T &v) const {
  auto it = vertices.find(v);
  if (it == vertices.end()) {
    throw std::runtime_error("Vertex not found");
  }
  return const_cast<T&>(*it);
}

template <typename T>
void DirectedGraph<T>::removeEdge(const T &from, const T &to) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(from, to))
    throw std::runtime_error("The edge does not exist");

  outAdjacency[from].erase(to);
  inAdjacency[to].erase(from);
  weights.erase({from, to});
}


template <typename T>
std::unordered_set<T>::const_iterator DirectedGraph<T>::begin() const {
  return vertices.begin();
}

template <typename T>
std::unordered_set<T>::const_iterator DirectedGraph<T>::end() const {
  return vertices.end();
}

template <typename T> int DirectedGraph<T>::getInDegree(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex is not in the graph");
  return inAdjacency.at(v).size();
}

template <typename T> int DirectedGraph<T>::getOutDegree(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex is not in the graph");
  return outAdjacency.at(v).size();
}

template <typename T>
OutboundEdgesIterator<T> DirectedGraph<T>::initOutboundEdgesIt(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex not in the graph");
  return OutboundEdgesIterator<T>(*this, v);
}

template <typename T>
InboundEdgesIterator<T> DirectedGraph<T>::initInboundEdgesIt(const T &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex not in the graph");
  return InboundEdgesIterator<T>(*this, v);
}

// TODO: simplify the iterators
template<typename T>
class OutboundEdgesIterator {
private:
  const DirectedGraph<T> &graph;
  const T &vertex;

public:
  class Iterator {
  private:
    typename std::unordered_set<T>::const_iterator it;
    const T &from;
    const DirectedGraph<T> &graph;

  public:
    Iterator(typename std::unordered_set<T>::const_iterator it, const T &from, const DirectedGraph<T> &graph)
      : it(it), from(from), graph(graph) {}

    Edge<T> operator*() const {
      return Edge<T>{from, *it, graph.getEdgeWeight(from, *it)};
    }

    Iterator& operator++() {
      ++it;
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return it == other.it;
    }

    bool operator!=(const Iterator& other) const {
      return it != other.it;
    }
  };

  OutboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
    : graph(graph), vertex(vertex) {}

  Iterator begin() const {
    return Iterator(graph.outAdjacency.at(vertex).begin(), vertex, graph);
  }

  Iterator end() const {
    return Iterator(graph.outAdjacency.at(vertex).end(), vertex, graph);
  }
};

template<typename T>
class InboundEdgesIterator {
private:
  const DirectedGraph<T> &graph; 
  const T& vertex;
public:
  class Iterator {
  private:
    typename std::unordered_set<T>::const_iterator it;
    const T& to;
    const DirectedGraph<T> &graph;

  public:
    Iterator(typename std::unordered_set<T>::const_iterator it, const T &to, const DirectedGraph<T> &graph)
      : it(it), to(to), graph(graph) {}

    Edge<T> operator*() const {
      return Edge<T>{*it, to, graph.getEdgeWeight(*it, to)};
    }

    Iterator& operator++() {
      ++it;
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return it == other.it;
    }

    bool operator!=(const Iterator& other) const {
      return it != other.it;
    }
  };

  InboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
    : graph(graph), vertex(vertex) {}

  Iterator begin() const {
    return Iterator(graph.inAdjacency.at(vertex).begin(), vertex, graph);
  }

  Iterator end() const {
    return Iterator(graph.inAdjacency.at(vertex).end(), vertex, graph);
  }
};
}// namespace graph
