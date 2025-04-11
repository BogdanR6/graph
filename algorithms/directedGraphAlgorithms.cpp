#include "../graph/directedGraph.hpp"
#include <queue>
#include <limits>

int lowestLengthFBfs(graph::DirectedGraph<int> &g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int>
  distance; // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[start] = 0;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto &v : g.getOutboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (v == end)
        return distance[t] + 1;

      if (!visited[v]) {
        visited[v] = true;
        distance[v] = distance[t] + 1;
        q.push(v);
      }
    }
  }

  return 999; // inf
}

int lowestLengthBBfs(graph::DirectedGraph<int> &g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int>
      distance; // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[end] = 0;
  visited[end] = true;
  q.push(end);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto &v : g.getInboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (v == start)
        return distance[t] + 1;

      if (!visited[v]) {
        visited[v] = true;
        distance[v] = distance[t] + 1;
        q.push(v);
      }
    }
  }

  return 999; // inf
}

// for next time problem 6
const int INF = std::numeric_limits<int>::max() / 2; // Avoid overflow in addition

// Computes the minimum cost walk between two vertices using matrix multiplication algorithm.
// Detects negative cost cycles.
void findLowestCostWalk(const graph::DirectedGraph<int>& g, int start, int end) {
  // Map vertex ID to index
  std::unordered_map<int, int> index;
  int i = 0;
  for (const auto& v : g)
      index[v] = i++;

  int n = g.getNrOfVertices();
  std::vector<std::vector<int>> cost(n, std::vector<int>(n, INF));
  std::vector<std::vector<int>> dist = cost;
  // Initialize cost matrix
  for (const auto& from : g) {
    for (const auto& to : g.getOutboundEdges(from)) {
      cost[index[from]][index[to]] = g.getEdgeCost(from, to);  
    }
  }

  dist = cost;

  // Matrix multiplication algorithm (Floyd-Warshall-style)
  for (int k = 0; k < n; ++k) {
    std::vector<std::vector<int>> newDist = dist;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (dist[i][k] < INF && dist[k][j] < INF) {
          newDist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
    dist = newDist;
  }

  // Detect negative cost cycles
  for (int i = 0; i < n; ++i) {
    if (dist[i][i] < 0) {
      std::cout << "Negative cost cycle detected in the graph.\n";
      return;
    }
  }

  int s = index[start];
  int t = index[end];

  if (dist[s][t] == INF) {
      std::cout << "No walk exists from vertex " << start << " to vertex " << end << ".\n";
  } else {
      std::cout << "Lowest cost walk from vertex " << start << " to vertex " << end
                << " is " << dist[s][t] << ".\n";
  }
}
