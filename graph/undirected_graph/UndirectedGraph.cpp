#include "UndirectedGraph.hpp"
#include <stdexcept>

namespace graph {

// TODO: Modify the EdgeHash class such that for undirected the direction does not matter

std::vector<Edge> UndirectedGraph::getEdges() const {
  std::vector<Edge> edgesV;
  for (const auto &edge : this->edges)
    edgesV.push_back(edge);
  return edgesV;
}


int UndirectedGraph::getEdgeWeight(const idT &fromId, const idT &toId) const {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");

  // the edge can go either way because the graph is undirected
  if (!isEdge(fromId, toId) && !isEdge(toId, fromId))
    throw std::runtime_error("The edge does not exists");

  auto edge_it = edges.find(Edge{fromId, toId});
  if (edge_it == edges.end())
    edge_it = edges.find(Edge{toId, fromId});
  if (edge_it == edges.end())
    throw std::runtime_error("The edge " + fromId + " -- " + toId + " does not exists in the edges set");

  return (*edge_it).weight;
}

GraphType UndirectedGraph::getGraphType() const {
  return GraphType::Undirected;
}

bool UndirectedGraph::isVertex(const idT &id) const {
  return adjacency.find(id) != adjacency.end();
}

bool UndirectedGraph::isEdge(const idT &fromId, const idT &toId) const {
  return isVertex(fromId) && isVertex(toId) &&
         adjacency.at(fromId).find(toId) != adjacency.at(fromId).end() &&
         (edges.find({fromId, toId}) != edges.end() || edges.find({toId, fromId}) != edges.end());
}

void UndirectedGraph::addVertex(const VertexSharedPtr &v) {
  if (isVertex(v->getId()))
    throw std::runtime_error("Vertex Already added");

  adjacency[v->getId()] = std::unordered_set<idT>();
  vertices.insert({v->getId(), v});
}

void UndirectedGraph::removeVertex(const idT &id) {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");
  
  auto adjacentToV = adjacency[id];
  for (const auto &ver : adjacentToV) {
    removeEdge(id, ver);
  }
  adjacency.erase(id);
  vertices.erase(id);
}

void UndirectedGraph::addEdge(const idT &fromId, const idT &toId, int weight) {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(fromId, toId))
    throw std::runtime_error("The edge already exists");

  adjacency.at(fromId).insert(toId);
  adjacency.at(toId).insert(fromId);
  edges.insert(Edge{fromId, toId, weight});
}

void UndirectedGraph::removeEdge(const idT &fromId, const idT &toId) {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(fromId, toId))
    throw std::runtime_error("The edge does not exist");

  adjacency[fromId].erase(toId);
  adjacency[toId].erase(fromId);
  auto edge_it = edges.find(Edge{fromId, toId});
  if (edge_it == edges.end())
    edge_it = edges.find(Edge{toId, fromId});
  if (edge_it == edges.end())
    throw std::runtime_error("The edge " + fromId + " -- " + toId + " does not exists in the edges set");
  edges.erase(edge_it);
}

const VertexSharedPtr &UndirectedGraph::getVertex(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");
  return vertices.at(id);
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

std::unordered_map<idT, VertexSharedPtr>::const_iterator UndirectedGraph::begin() const {
  return vertices.begin();
}

std::unordered_map<idT, VertexSharedPtr>::const_iterator UndirectedGraph::end() const {
  return vertices.end();
}


AdjacentEdgesView UndirectedGraph::getAdjacentEdges(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex is not in the graph");
  AdjacentEdgesView view;
  auto it = adjacency.find(id);
  if (it != adjacency.end()) {
      // Populate edges
      for (const idT& toId : it->second)
        view.addEdge(Edge{id, toId, getEdgeWeight(id, toId)});
  }
  return view;
}

} //namespace graph
