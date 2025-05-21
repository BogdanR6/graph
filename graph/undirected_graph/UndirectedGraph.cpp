#include "UndirectedGraph.hpp"
#include "iterators/Iterators.hpp"
#include <stdexcept>

namespace graph {

// TODO: Modify the EdgeHash class such that for undirected the direction does not matter

std::vector<Edge> UndirectedGraph::getEdges() const {
  std::vector<Edge> edgesV;
  for (const auto &edge : this->edges)
    edgesV.push_back(edge);
  return edgesV;
}


int UndirectedGraph::getEdgeWeight(const VertexSharedPtr &from, const VertexSharedPtr &to) const {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");

  // the edge can go either way because the graph is undirected
  if (!isEdge(from, to) && !isEdge(to, from))
    throw std::runtime_error("The edge does not exists");

  auto edge_it = edges.find(Edge{from, to});
  if (edge_it == edges.end())
    edge_it = edges.find(Edge{to, from});
  if (edge_it == edges.end())
    throw std::runtime_error("The edge " + *from + " -- " + *to + " does not exists in the edges set");

  return (*edge_it).weight;
}

GraphType UndirectedGraph::getGraphType() const {
  return GraphType::Undirected;
}

bool UndirectedGraph::isVertex(const VertexSharedPtr &v) const {
  return adjacency.find(v) != adjacency.end();
}

bool UndirectedGraph::isEdge(const VertexSharedPtr &from, const VertexSharedPtr &to) const {
  return isVertex(from) && isVertex(to) &&
         adjacency.at(from).find(to) != adjacency.at(from).end() &&
         (edges.find({from, to}) != edges.end() || edges.find({to, from}) != edges.end());
}

void UndirectedGraph::addVertex(const VertexSharedPtr &v) {
  if (isVertex(v))
    throw std::runtime_error("Vertex Already added");

  adjacency[v] = std::unordered_set<VertexSharedPtr>();
  vertices.insert(v);
}

void UndirectedGraph::removeVertex(const VertexSharedPtr &v) {
  if (!isVertex(v))
    throw std::runtime_error("Vertex not in the graph");
  
  auto adjacentToV = adjacency[v];
  for (const auto &ver : adjacentToV) {
    removeEdge(v, ver);
  }
  adjacency.erase(v);
  vertices.erase(v);
}

void UndirectedGraph::addEdge(const VertexSharedPtr &from, const VertexSharedPtr &to, const int &weight) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(from, to))
    throw std::runtime_error("The edge already exists");

  adjacency.at(from).insert(to);
  adjacency.at(to).insert(from);
  edges.insert(Edge{from, to, weight});
}

void UndirectedGraph::removeEdge(const VertexSharedPtr &from, const VertexSharedPtr &to) {
  if (!isVertex(from))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(to))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(from, to))
    throw std::runtime_error("The edge does not exist");

  adjacency[from].erase(to);
  adjacency[to].erase(from);
  auto edge_it = edges.find(Edge{from, to});
  if (edge_it == edges.end())
    edge_it = edges.find(Edge{to, from});
  if (edge_it == edges.end())
    throw std::runtime_error("The edge " + *from + " -- " + *to + " does not exists in the edges set");
  edges.erase(edge_it);
}

VertexSharedPtr &UndirectedGraph::getVertex(const VertexSharedPtr &v) const {
  auto it = vertices.find(v);
  if (it == vertices.end()) {
    throw std::runtime_error("Vertex not found");
  }
  return const_cast<VertexSharedPtr&>(*it);
}

int UndirectedGraph::getNrOfVertices() const {
  return vertices.size();
}

int UndirectedGraph::getNrOfEdges() const {
  int count = 0;
  for (const auto &[_, neighbors] : adjacency) {
    count += neighbors.size();
  }
  return count / 2;
}

void UndirectedGraph::clear() {
  adjacency.clear();
  vertices.clear();
}

std::unordered_set<VertexSharedPtr>::const_iterator UndirectedGraph::begin() const {
  return vertices.begin();
}

std::unordered_set<VertexSharedPtr>::const_iterator UndirectedGraph::end() const {
  return vertices.end();
}


AdjacentEdgesView UndirectedGraph::getAdjacentEdges(const VertexSharedPtr &v) const {
  if (!isVertex(v))
    throw std::runtime_error("Vertex is not in the graph");
  return AdjacentEdgesView(*this, v);
}

} //namespace graph
