#include "DirectedGraphAlgorithms.hpp"
#include "../directed_graph/DirectedGraph.hpp"
#include "../directed_graph/iterators/Iterators.hpp"
#include <algorithm>
#include <queue>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <stack>

namespace graph {
namespace algorithms {

int lowestLengthFBfs(graph::DirectedGraph &g, const idT &startId, const idT &endId) {
  std::unordered_map<idT, bool> visited;
  std::unordered_map<idT, int> distance; // Stores the shortest distance from start to each node
  std::queue<idT> q;

  distance[startId] = 0;
  visited[startId] = true;
  q.push(startId);
  while (!q.empty()) {
    const idT currentVertexId = q.front();
    q.pop();

    for (const auto &[_, toId, __] : g.initOutboundEdgesIt(currentVertexId)) {
      // If we have reached the end node, return the distance
      if (toId == endId)
        return distance[currentVertexId] + 1;

      if (!visited[toId]) {
        visited[toId] = true;
        distance[toId] = distance[currentVertexId] + 1;
        q.push(toId);
      }
    }
  }

  return 999; // inf
}

int lowestLengthBBfs(graph::DirectedGraph &g, const idT &startId, const idT &endId) {
  std::unordered_map<idT, bool> visited;
  std::unordered_map<idT, int> distance; // Stores the shortest distance from start to each node
  std::queue<idT> q;

  distance[endId] = 0;
  visited[endId] = true;
  q.push(endId);
  while (!q.empty()) {
    const idT currentVertex = q.front();
    q.pop();

    for (const auto &[_, toId, __] : g.initInboundEdgesIt(currentVertex)) {
      // If we have reached the end node, return the distance
      if (toId == startId)
        return distance[currentVertex] + 1;

      if (!visited[toId]) {
        visited[toId] = true;
        distance[toId] = distance[currentVertex] + 1;
        q.push(toId);
      }
    }
  }

  return 999; // inf
}

// 3.6
const int INF = std::numeric_limits<int>::max() / 2; // Avoid overflow in addition

// Computes the minimum cost walk between two vertices using matrix multiplication algorithm.
// Detects negative cost cycles.
struct WalkResult {
  std::vector<std::vector<int>> dist; // Cost of the shortest path from vertex i to vertex j
  std::vector<std::vector<int>> pred; // Predecessor of vertex j on the shortest path from i
  std::unordered_map<idT, int> index;
  std::vector<idT> reverseIndex;
};

WalkResult findLowestCostWalk(const graph::DirectedGraph &g) {
  const int INF = std::numeric_limits<int>::max() / 2;

  std::unordered_map<std::string, int> index;
  std::vector<std::string> reverseIndex;
  int i = 0;
  for (const auto &[vertexId, _] : g) {
    index[vertexId] = i++;
    reverseIndex.push_back(vertexId);
  }

  int n = g.getNrOfVertices();
  std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
  std::vector<std::vector<int>> pred(n, std::vector<int>(n, -1));

  // Initialize cost and predecessor matrices
  for (const auto &[vertexId, _] : g) {
    dist[index[vertexId]][index[vertexId]] = 0;
    pred[index[vertexId]][index[vertexId]] = index[vertexId];
    for (const auto &[fromId, toId, __] : g.initOutboundEdgesIt(vertexId)) {
      int fromIndex = index[fromId];
      int toIndex = index[toId];
      dist[fromIndex][toIndex] = g.getEdgeWeight(fromId, toId);

      pred[fromIndex][toIndex] = fromIndex;
    }
  }

  // Floyd-Warshall + path reconstruction
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          pred[i][j] = pred[k][j];
        }
      }
    }
  }

  // Detect negative cycles
  for (int i = 0; i < n; ++i) {
    if (dist[i][i] < 0)
      throw std::runtime_error("Negative cost cycle detected.");
  }

  return {dist, pred, index, reverseIndex};
}

std::pair<std::vector<idT>, int> reconstructWalk(const WalkResult &result, const idT &startId, const idT &endId) {
  int startIndex = result.index.at(startId);
  int endIndex = result.index.at(endId);
  if (result.dist[startIndex][endIndex] == std::numeric_limits<int>::max() / 2)
    return {}; // No path

  std::vector<int> pathIndices;
  for (int currIndex = endIndex; currIndex != startIndex; currIndex = result.pred[startIndex][currIndex]) {
    if (currIndex == -1)
      return {}; // No path
    pathIndices.push_back(currIndex);
  }
  pathIndices.push_back(startIndex);
  std::reverse(pathIndices.begin(), pathIndices.end());

  std::vector<std::string> path;
  for (int idx : pathIndices)
    path.push_back(result.reverseIndex[idx]);
  return std::make_pair(path, result.dist[startIndex][endIndex]);
}

std::pair<std::vector<idT>, int> getLowestCostWalk(const graph::DirectedGraph &g, const idT &startId, const idT &endId) {
  auto result = findLowestCostWalk(g);
  return reconstructWalk(result, startId, endId);
}

/*
* 4.2 Write a program that, given a list of activities with duration and list of prerequisites for each activity, does the following:
*
*   verify if the corresponding graph is a DAG and performs a topological sorting of the activities using the algorithm based on predecessor counters;
*   prints the earliest and the latest starting time for each activity and the total time of the project.
*   prints the critical activities. 
*/
struct ActivitiesData {
  std::unordered_map<idT, int> erliestStart;
  std::unordered_map<idT, int> latestStart;
  std::vector<idT> topologicalOrder;
};

std::vector<idT> getTopologicalOrder(const graph::DirectedGraph &g) {
  // Kahn's algorithm
  graph::DirectedGraph aux = g;
  std::vector<idT> sortedVertices;
  std::stack<idT> startingVertices;
  for (const auto &[vertexId, _] : aux) {
    if (aux.initInboundEdgesIt(vertexId).begin() == aux.initInboundEdgesIt(vertexId).end()) { // no inbound edges
      startingVertices.push(vertexId);
    }
  }

  if (startingVertices.empty())
    return {}; // cycle or empty

  while (!startingVertices.empty()) {
    auto fromId = startingVertices.top(); startingVertices.pop();
    sortedVertices.push_back(fromId);
    for (const auto &toId : aux.getAllOutboundVertices(fromId)) {
      aux.removeEdge(fromId, toId);
      if (aux.initInboundEdgesIt(toId).begin() == aux.initInboundEdgesIt(toId).end()) { // no inbound edges
        startingVertices.push(toId);
      }
    }
  }

  if (aux.getNrOfEdges() != 0)
    return {}; // cycle

  return sortedVertices;
}

} // namespace algorithms
} // namespace graph
