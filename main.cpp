#include <iostream>
#include <assert.h>
// #include "interface/undirectedGraph.hpp"
#include "interface/directedGraph.hpp"
#include <queue>

int lowest_length_bfs(graph::DirectedGraph<int>& g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int> distance;  // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[start] = 0;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto& v : g.getOutboundEdges(t)) {
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

  return 999;
}


int main() {
	graph::DirectedGraph<int> g;
	g.addVertex(1);
	g.addVertex(2);
	g.addVertex(3);
	g.addVertex(4);
	g.addVertex(5);
	g.addVertex(6);
	g.addVertex(7);

	g.addEdge(3, 2);
	g.addEdge(1, 4);
	g.addEdge(5, 3);
	g.addEdge(6, 2);
	g.addEdge(4, 5);

	for (const auto& v : g) {
	  std::cout << v << ' ';
	}

	std::cout << '\n';
	std::cout << lowest_length_bfs(g, 1, 2) << '\n';
	return 0;
}
