#include "GraphService.hpp"
#include "../errors/InvalidInputError.cpp"
#include <string>


void GraphService::addVertex(const int& vertexId) {
  graph->addVertex(vertexId);
}


void GraphService::removeVertex(const int &vertexId) {
  graph->removeVertex(vertexId);
}


bool GraphService::isVertex(const int &vertexId) {
  return graph->isVertex(vertexId);
}
  

void GraphService::addEdge(const int &fromVertexId, const int &toVertexId, const int &weight) {
  graph->addEdge(fromVertexId, toVertexId, weight);
}


void GraphService::removeEdge(const int &fromVertexId, const int &toVertexId) {
  graph->removeEdge(fromVertexId, toVertexId);
}


bool GraphService::isEdge(const int &fromVertexId, const int &toVertexId) {
  return graph->isEdge(fromVertexId, toVertexId);
}


std::string GraphService::getVertices() {
  std::string vertices = "";
  for (const auto& vertex : *graph) {
    vertices += std::to_string(vertex) + " ";
  }
  return vertices; 
}


std::string GraphService::getAdjacentVertices(const int &vertexId) {
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw InvalidOperationOnGraphType("Adjacent Vertices are available only for Undirected graphs");
  std::string vertices = "The adjacent vertices of " + std::to_string(vertexId) + " are:\n";
  auto *undirected = dynamic_cast<graph::UndirectedGraph<int>*>(graph.get());
  int count = 0;
  for (const auto& vertex : undirected->getAdjacentVertices(vertexId)) {
    vertices += std::to_string(vertex) + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + std::to_string(vertexId) + " has no adjacent vertices\n";
  }
  return vertices; 
}


std::string GraphService::getOutboundVertices(const int &vertexId) {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Outbound Vertices are available only for Directed graphs");

  std::string vertices = "The outbound vertices of " + std::to_string(vertexId) + " are:\n";
  auto *directed = dynamic_cast<graph::DirectedGraph<int>*>(graph.get());
  int count = 0;
  for (const auto& vertex : directed->getOutboundEdges(vertexId)) {
    vertices += std::to_string(vertex) + " ";
    ++count;
  }
  if (count == 0) {
    vertices = "The vertex " + std::to_string(vertexId) + " has no outbound vertices\n";
  }
  return vertices;
}


std::string GraphService::getEdges() {
  return "Edges...";
}
