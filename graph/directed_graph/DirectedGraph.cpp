#include "DirectedGraph.hpp"
#include "iterators/Iterators.hpp"

namespace graph {

/* Returns the type of the graph */
GraphType DirectedGraph::getGraphType() const {
  return GraphType::Directed;
}

// Methods on vertices

/* Returns true if the vertex is in the graph, else false */
bool DirectedGraph::isVertex(const idT &id) const {
  return vertices.find(id) != vertices.end();
}


/* Adds the vertex to the graph */
void DirectedGraph::addVertex(const VertexSharedPtr &v) {
  if (isVertex(v->getId()))
    throw std::runtime_error("Vertex Already added");

  outAdjacency[v->getId()] = std::unordered_set<idT>();
  inAdjacency[v->getId()] = std::unordered_set<idT>();
  vertices.insert({v->getId(), v});
}


/* Removes the vertex from the graph */
void DirectedGraph::removeVertex(const idT &id) {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");

  for (const auto &ver : outAdjacency[id]) {
    inAdjacency[ver].erase(id);
  }
  outAdjacency.erase(id);
  vertices.erase(id);
}


/* Returns the shared ptr to the given vertex (useful for when only the id of the vertex is known) */
const VertexSharedPtr &DirectedGraph::getVertex(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");
  return vertices.at(id);
}


/* Returns the number of vertices */
int DirectedGraph::getNrOfVertices() const {
  return vertices.size();
}


std::unordered_set<idT> DirectedGraph::getAllOutboundVertices(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("v is not in the graph");

  return outAdjacency.at(id);
}


std::unordered_set<idT> DirectedGraph::getAllInboundVertices(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("v is not in the graph");

  return inAdjacency.at(id);
}


// Methods on edges

/* Returns true if the edge is in the graph, else false */
bool DirectedGraph::isEdge(const idT &fromId, const idT &toId) const {
  return isVertex(fromId) && isVertex(toId) &&
         outAdjacency.at(fromId).find(toId) != outAdjacency.at(fromId).end();
}


/* Adds the edge to the graph */
void DirectedGraph::addEdge(const idT &fromId, const idT &toId, int weight) {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");
  if (isEdge(fromId, toId))
    throw std::runtime_error("The edge already exists");

  outAdjacency[fromId].insert(toId);
  inAdjacency[toId].insert(fromId);
  weights.insert({fromId, toId, weight});
}


/* Removes the edge from the graph */
void DirectedGraph::removeEdge(const idT &fromId, const idT &toId) {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(fromId, toId))
    throw std::runtime_error("The edge does not exist");

  outAdjacency[fromId].erase(toId);
  inAdjacency[toId].erase(fromId);
  weights.erase({fromId, toId});
}


/* Returns the number of edges */
int DirectedGraph::getNrOfEdges() const {
  return weights.size();
}


/* Returns the weigth of the edge between _from_ and _to_ */
int DirectedGraph::getEdgeWeight(const idT &fromId, const idT &toId) const {
  if (!isVertex(fromId))
    throw std::runtime_error("from is not in the graph");
  if (!isVertex(toId))
    throw std::runtime_error("to is not in the graph");
  if (!isEdge(fromId, toId))
    throw std::runtime_error("The edge does not exit exists");
  return weights.find({fromId, toId})->weight;
}


/* Returns a vector of all the edges in the graph */
std::vector<Edge> DirectedGraph::getEdges() const {
  std::vector<Edge> edges;
  for (const auto &edge : weights)
    edges.push_back(edge);
  return edges;
}


// Iterators 

/* Returns a constant iterator to the begining of the vertices (the order is not guaranteed) */
std::unordered_map<idT, VertexSharedPtr>::const_iterator DirectedGraph::begin() const {
  return vertices.begin();
}


/* Returns a constant iterator to the end of the vertices */
std::unordered_map<idT, VertexSharedPtr>::const_iterator DirectedGraph::end() const {
  return vertices.end();
}


/* Creates a new Iterator over the outbound edges (relative to the given vertex) */
OutboundEdgesIterator DirectedGraph::initOutboundEdgesIt(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");
  return OutboundEdgesIterator(*this, id);
}


/* Creates a new Iterator over the inbound edges (relative to the given vertex) */
InboundEdgesIterator DirectedGraph::initInboundEdgesIt(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex not in the graph");
  return InboundEdgesIterator(*this, id);
}

AdjacentEdgesView DirectedGraph::getAdjacentEdges(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex is not in the graph");

  AdjacentEdgesView view;

  for (const auto &toId : outAdjacency.at(id)) {
    view.addEdge(Edge{id, toId, getEdgeWeight(id, toId)});
  }
  for (const auto &fromId : inAdjacency.at(id)) {
    view.addEdge(Edge{fromId, id, getEdgeWeight(fromId, id)});
  }

  return view;
}


// Misc Methods

/* Clear all the edges and vertices from the graph */
void DirectedGraph::clear() {
  inAdjacency.clear();
  outAdjacency.clear();
  vertices.clear();
  weights.clear();
}


/* Returns the in degree of the given vertex */
int DirectedGraph::getInDegree(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex is not in the graph");
  return inAdjacency.at(id).size();
}


/* Returns the out degree of the given vertex */
int DirectedGraph::getOutDegree(const idT &id) const {
  if (!isVertex(id))
    throw std::runtime_error("Vertex is not in the graph");
  return outAdjacency.at(id).size();
}


} // namespace graph
