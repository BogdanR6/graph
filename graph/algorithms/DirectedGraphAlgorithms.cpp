#include "DirectedGraphAlgorithms.hpp"
#include "../DirectedGraph.hpp"
#include <algorithm>
#include <queue>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

namespace graph {
namespace algorithms {
int lowestLengthFBfs(graph::DirectedGraph<std::string> &g, std::string start, std::string end) {
  std::unordered_map<std::string, bool> visited;
  std::unordered_map<std::string, int> distance; // Stores the shortest distance from start to each node
  std::queue<std::string> q;

  distance[start] = 0;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    std::string t = q.front();
    q.pop();

    for (const auto &[_, to, __] : g.getOutboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (to == end)
        return distance[t] + 1;

      if (!visited[to]) {
        visited[to] = true;
        distance[to] = distance[t] + 1;
        q.push(to);
      }
    }
  }

  return 999; // inf
}

int lowestLengthBBfs(graph::DirectedGraph<std::string> &g, std::string start, std::string end) {
  std::unordered_map<std::string, bool> visited;
  std::unordered_map<std::string, int> distance; // Stores the shortest distance from start to each node
  std::queue<std::string> q;

  distance[end] = 0;
  visited[end] = true;
  q.push(end);
  while (!q.empty()) {
    std::string t = q.front();
    q.pop();

    for (const auto &[_, to, __] : g.getInboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (to == start)
        return distance[t] + 1;

      if (!visited[to]) {
        visited[to] = true;
        distance[to] = distance[t] + 1;
        q.push(to);
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
  std::vector<std::vector<int>> dist;
  std::vector<std::vector<int>> pred;
  std::unordered_map<std::string, int> index;
  std::vector<std::string> reverseIndex;
};

WalkResult findLowestCostWalk(const graph::DirectedGraph<std::string> &g) {
  const int INF = std::numeric_limits<int>::max() / 2;

  std::unordered_map<std::string, int> index;
  std::vector<std::string> reverseIndex;
  int i = 0;
  for (const auto &v : g) {
    index[v] = i++;
    reverseIndex.push_back(v);
  }

  int n = g.getNrOfVertices();
  std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
  std::vector<std::vector<int>> pred(n, std::vector<int>(n, -1));

  // Initialize cost and predecessor matrices
  for (const auto &v : g) {
    dist[index[v]][index[v]] = 0;
    pred[index[v]][index[v]] = index[v];
    for (const auto &[_, to, __] : g.getOutboundEdges(v)) {
      int u = index[v];
      int v_ = index[to];
      dist[u][v_] = g.getEdgeWeight(v, to);

      // to be removed
      for (const auto &row : dist) {
        for (const auto &col : row) {
          if (col == INF) std::cout << "I ";
          else std::cout << col << " ";
        }
        std::cout << "\n";
      }
      std::cout << "\n";
      // 'till here
      
      pred[u][v_] = u;
    }
  }

  // Floyd-Warshall with path reconstruction
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

std::pair<std::vector<std::string>, int> reconstructWalk(const WalkResult &result, const std::string &start, const std::string &end) {
  int s = result.index.at(start);
  int t = result.index.at(end);
  if (result.dist[s][t] == std::numeric_limits<int>::max() / 2)
    return {}; // No path

  std::vector<int> pathIndices;
  for (int at = t; at != s; at = result.pred[s][at]) {
    if (at == -1)
      return {}; // No path
    pathIndices.push_back(at);
  }
  pathIndices.push_back(s);
  std::reverse(pathIndices.begin(), pathIndices.end());

  std::vector<std::string> path;
  for (int idx : pathIndices)
    path.push_back(result.reverseIndex[idx]);
  return std::make_pair(path, result.dist[s][t]);
}
std::pair<std::vector<std::string>, int> getLowestCostWalk(const graph::DirectedGraph<std::string> &g, const std::string &start, const std::string &end) {
  auto result = findLowestCostWalk(g);
  return reconstructWalk(result, start, end);
}

} // namespace algorithms
} // namespace graph
