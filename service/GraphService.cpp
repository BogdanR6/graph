#include "../graph/directed_graph/iterators/Iterators.hpp"
#include "GraphService.hpp"
#include "../graph/vertices/StringVertex.hpp"
#include "../errors/InvalidInputError.cpp"
#include "../graph/directed_graph/DirectedGraph.hpp"
#include "../graph/special/ActivityGraph.hpp"
#include "../graph/algorithms/UndirectedGraphAlgorithms.hpp"
#include "../graph/algorithms/DirectedGraphAlgorithms.hpp"
#include "../graph/special/ActivityGraph.hpp"
#include "../graph/abstract/Graph.hpp"
#include "../graph/vertices/StringVertex.hpp"
#include "../graph/vertices/ActivityVertex.hpp"
#include "../graph/undirected_graph/UndirectedGraph.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>

graph::GraphType GraphService::getGraphType() const {
  return graph->getGraphType();
}

void GraphService::addVertex(const graph::VertexSharedPtr &vertex) {
  graph->addVertex(vertex);
}


void GraphService::removeVertex(const graph::idT &vertexId) {
  graph->removeVertex(vertexId);
}


bool GraphService::isVertex(const graph::idT &vertexId) {
  return graph->isVertex(vertexId);
}
  

void GraphService::addEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId, int weight) {
  graph->addEdge(fromVertexId, toVertexId, weight);
}


void GraphService::removeEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId) {
  graph->removeEdge(fromVertexId, toVertexId);
}


bool GraphService::isEdge(const graph::idT &fromVertexId, const graph::idT &toVertexId) {
  return graph->isEdge(fromVertexId, toVertexId);
}


std::vector<graph::VertexSharedPtr> GraphService::getVertices() {
  std::vector<graph::VertexSharedPtr> vertices;
  for (const auto& [_, vertexPtr] : *graph) {
    vertices.push_back(vertexPtr);
  }
  return vertices; 
}


std::vector<graph::Edge> GraphService::getAdjacentEdges(const graph::idT &vertexId) const {
  return graph->getAdjacentEdges(vertexId).getAll();
}


std::vector<graph::Edge> GraphService::getOutboundEdges(const graph::idT &vertexId) const {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Outbound Vertices are defined only for Directed graphs");
  auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  std::vector<graph::Edge> edges;
  for (const graph::Edge& edge : directed->initOutboundEdgesIt(vertexId)) {
    edges.push_back(edge);
  }
  return edges;
}


std::vector<graph::Edge> GraphService::getInboundEdges(const graph::idT &vertexId) const { 
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw InvalidOperationOnGraphType("Inbound Vertices are defined only for Directed graphs");
  auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  std::vector<graph::Edge> edges;
  for (const graph::Edge& edge : directed->initInboundEdgesIt(vertexId)) {
    edges.push_back(edge);
  }
  return edges;
}


std::vector<graph::Edge> GraphService::getEdges() {
  return graph->getEdges();
}

void GraphService::loadGraph(const std::string &path, const std::string &graphType) {
  std::ifstream fin(path);
  if (!fin.is_open())
    throw std::runtime_error("Could not open file '" + path + "' for reading");

  //chose the graph type
  if (graphType == "undirected") {
    graph = std::make_shared<graph::UndirectedGraph>();
  } else if (graphType == "directed") {
    graph = std::make_shared<graph::DirectedGraph>();
  } else if (graphType == "activity") {
    graph = std::make_shared<graph::special::ActivityGraph>();
  } else {
    throw std::runtime_error("'" + graphType + "' is not a valid graph type");
  }
  graph->clear();

  // custom split function
  auto split = [](const std::string &str, const std::string &separator = " ") -> std::vector<std::string> {
    if (str.empty())
      return {};

    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end;

    while ((end = str.find(separator, start)) != std::string::npos) {
      tokens.push_back(str.substr(start, end - start));
      start = end + separator.length();
    }
    tokens.push_back(str.substr(start)); // add the last token
    return tokens;
  };

  std::string separator = " ";
  if (graphType == "activity")
    separator = " | ";

  std::string line;
  if (!std::getline(fin, line))
    throw std::runtime_error("Empty file!");

  std::vector<std::string> firstLine = split(line, separator);

  if (graphType == "activity") {
    std::unordered_multimap<graph::idT, graph::idT> edgesToAdd;
    std::unordered_set<graph::idT> finalActivities;
    auto addActivity = [&](std::vector<std::string> l) {
      if (l.size() == 0)
        return;
      if (l.size() != 4 && l.size() != 3) {
        throw std::runtime_error("Invalid format for loading an Activity Graph!");
        // TODO: maybe clear the graph if errors are thrown durring loading
      }
      int idIndex = 0;
      int nameIndex = 1;
      int durationIndex = 2;
      int inBoundIndex = 3; 
      if (l.size() == 3) {
        nameIndex = -1;
        durationIndex = 1;
        inBoundIndex = 2;
      }
      graph::idT activityId = l[idIndex];
      std::string activityName = l.size() == 4 ? l[nameIndex] : "";
      int activityDuration = std::stoi(l[durationIndex]);
      std::vector<graph::idT> inBound = split(l[inBoundIndex], ",");
      auto activity = std::make_shared<graph::special::Activity>(activityId, activityName, activityDuration);
      graph->addVertex(activity);
      finalActivities.insert(activityId);
      for (auto adjId : inBound) {
        if (adjId == "-") // replace - with X (the start)
          adjId = "X";
        edgesToAdd.insert({adjId, activityId});
        if (finalActivities.find(adjId) != finalActivities.end()) finalActivities.erase(adjId);
      }
    };

    addActivity({"X", "Start", "0", ""}); // fictiv first Activity 
    addActivity(firstLine);
    while (std::getline(fin, line)) {
      auto tokens = split(line, separator);
      addActivity(tokens);
    }

    std::string finalActivitiesStr = "";
    for (const auto &id : finalActivities)
      finalActivitiesStr += std::format("{},", id);
    finalActivitiesStr = finalActivitiesStr.substr(0, finalActivitiesStr.size() - 1);
    addActivity({"Y", "Final", "0", finalActivitiesStr}); // fictiv final Activity
    
    for (const auto &[fromId, toId] : edgesToAdd)
      graph->addEdge(fromId, toId);
  } else if (firstLine.size() == 2 &&
    std::all_of(firstLine[0].begin(), firstLine[0].end(), ::isdigit) &&
    std::all_of(firstLine[1].begin(), firstLine[1].end(), ::isdigit)) {
    // Format 1: vertex_count edge_count
    int vertexCount = std::stoi(firstLine[0]);
    for (int i = 0; i < vertexCount; ++i)
        graph->addVertex(std::make_shared<graph::StringVertex>(std::to_string(i)));
      
    int edgeCount = std::stoi(firstLine[1]);

    for (int i = 0; i < edgeCount; ++i) {
      if (!std::getline(fin, line))
        throw std::runtime_error("Unexpected end of file while reading edges");
      auto tokens = split(line);
      if (tokens.size() != 3)
        throw std::runtime_error("Expected format 'from to cost' on line " + std::to_string(i + 2));

      graph::idT fromId = tokens[0];
      graph::idT toId = tokens[1];
      int cost = std::stoi(tokens[2]);

      try {
        graph->addVertex(std::make_shared<graph::StringVertex>(fromId));
      } catch (...){} // ignore if already exists
      try {
        graph->addVertex(std::make_shared<graph::StringVertex>(toId));
      } catch (...){} // ignore if already exists
      try {
        graph->addEdge(fromId, toId, cost);
      } catch (...){} // ignore if already exists
    }
  } else {
    bool hasCost = firstLine.size() == 3;

    do {
      auto tokens = firstLine;  // the split line 

      if (tokens.size() == 1) {
        // Single vertex: isolated vertex
        graph::idT vertexId = tokens[0];
        try {
        graph->addVertex(std::make_shared<graph::StringVertex>(vertexId));
        } catch (...) {} // ignore if already exists
      } 
      else if (tokens.size() == 2 || tokens.size() == 3) {
        // Edge: from to [cost]
        graph::idT fromId = tokens[0];
        graph::idT toId = tokens[1];
        int cost = 1;
        if (tokens.size() == 3)
          cost = std::stoi(tokens[2]);

        try {
            graph->addVertex(std::make_shared<graph::StringVertex>(fromId));
        } catch (...) {}
        try {
            graph->addVertex(std::make_shared<graph::StringVertex>(toId));
        } catch (...) {}
        try {
            graph->addEdge(fromId, toId, cost);
        } catch (...) {}
      } 
      else {
          throw std::runtime_error("Invalid line format: '" + line + "'");
      }

    } while (std::getline(fin, line) && (firstLine = split(line), !line.empty()));
  }
}


void GraphService::saveGraph(const std::string& path) const {
  std::ofstream fout(path);
  if (!fout.is_open())
    throw std::runtime_error("Could not open file '" + path + "' for writing");

  if (graph->getGraphType() == graph::GraphType::Directed) {
    auto *directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
    for (const auto &[fromId, _] : *directed) {
      for (const auto &[__, to, cost] : directed->initOutboundEdgesIt(fromId))
          fout << fromId << " " << to << " " << cost << "\n";
    }
    for (const auto& [vertexId, _] : *directed) {
      auto outEdges = directed->initOutboundEdgesIt(vertexId);
      auto inEdges = directed->initInboundEdgesIt(vertexId);

      if (outEdges.begin() == outEdges.end() && inEdges.begin() == inEdges.end()) {
        fout << vertexId << "\n";
      }
    }
  }

  else if (graph->getGraphType() == graph::GraphType::Undirected) {
    auto *undirected = dynamic_cast<graph::UndirectedGraph*>(graph.get());
    for (const auto &[vertexId, _] : *undirected) {
      auto adjacentEdges = undirected->getAdjacentEdges(vertexId);
      if (adjacentEdges.empty())
        fout << vertexId << "\n";
      else for (const auto &edge : adjacentEdges) {
          fout << edge.fromId << " " << edge.toId << " " << edge.weight << "\n";
      }
    }
  }
  fout.close();
}


std::vector<graph::UndirectedGraph> GraphService::getConnectedComponentsOfUnorderedGraph() const {
  if (graph->getGraphType() != graph::GraphType::Undirected)
    throw std::runtime_error("getConnectedComponentsOfUndirectedGraph is only available for undirected graphs");
  auto undirected = dynamic_cast<graph::UndirectedGraph*>(graph.get());
  return graph::algorithms::getConnectedComponentsDFS(*undirected);
}


std::pair<std::vector<graph::idT>, int> GraphService::getLowestCostWalk(const graph::idT &startId, const graph::idT &endId) const {
  if (graph->getGraphType() != graph::GraphType::Directed)
    throw std::runtime_error("getLowestCostWalk is only available for directed graphs");
  auto directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  return graph::algorithms::getLowestCostWalk(*directed, startId, endId);
}


std::vector<graph::idT> GraphService::topologicalSort() const {
  if (graph->getGraphType() != graph::GraphType::Directed) 
    throw std::runtime_error("topologicalSort is only available for directed graphs");
  auto directed = dynamic_cast<graph::DirectedGraph*>(graph.get());
  return graph::algorithms::getTopologicalOrder(*directed);
}
