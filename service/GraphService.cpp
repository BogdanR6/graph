#include "GraphService.hpp"
#include "../errors/InvalidInputError.cpp"
#include "../graph/DirectedGraph.hpp"
#include <string>


void GraphService::addVertex(const TElem& vertexId) {
  graph->addVertex(vertexId);
}


void GraphService::removeVertex(const TElem &vertexId) {
  graph->removeVertex(vertexId);
}


bool GraphService::isVertex(const TElem &vertexId) {
  return graph->isVertex(vertexId);
}
  

void GraphService::addEdge(const TElem &fromVertexId, const TElem &toVertexId, const int &weight) {
  graph->addEdge(fromVertexId, toVertexId, weight);
}


void GraphService::removeEdge(const TElem &fromVertexId, const TElem &toVertexId) {
  graph->removeEdge(fromVertexId, toVertexId);
}


bool GraphService::isEdge(const TElem &fromVertexId, const TElem &toVertexId) {
  return graph->isEdge(fromVertexId, toVertexId);
}


std::string GraphService::getVertices() {
  std::string vertices = "";
  for (const auto& vertex : *graph) {
    vertices += vertex + " ";
  }
  return vertices; 
}


std::string GraphService::getAdjacentVertices(const TElem &vertexId) {
  // TODO: add support for all types of graphs 
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw InvalidOperationOnGraphType("Adjacent Vertices are available only for Undirected graphs");
  std::string vertices = "The adjacent vertices of " + vertexId + " are:\n";
  auto *undirected = dynamic_cast<graph::UndirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& vertex : undirected->getAdjacentVertices(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no adjacent vertices\n";
  }
  return vertices; 
}


std::string GraphService::getOutboundVertices(const TElem &vertexId) {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Outbound Vertices are defined only for Directed graphs");
  std::string vertices = "The outbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& vertex : directed->getOutboundVertices(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no outbound vertices\n";
  }
  return vertices;
}


std::string GraphService::getOutboundInbound(const TElem &vertexId) {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Inbound Vertices are defined only for Directed graphs");
  std::string vertices = "The inbound vertices of " + vertexId + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph<TElem>*>(graph.get());
  int count = 0;
  for (const auto& vertex : directed->getInboundVertices(vertexId)) {
    vertices += vertex + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + vertexId + " has no inbound vertices\n";
  }
  return vertices;
}


std::string GraphService::getEdges() {
  return "Edges...";
}
