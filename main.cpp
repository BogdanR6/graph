#include <iostream>
#include <assert.h>
#include "interface/undirectedGraph.hpp"
#include "interface/directedGraph.hpp"
#include <queue>
#include <stack>
#include <fstream>
#include <ranges>

int lowestLengthFBfs(graph::DirectedGraph<int>& g, int start, int end) {
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

  return 999; //inf
}

int lowestLengthBBfs(graph::DirectedGraph<int>& g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int> distance;  // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[end] = 0;
  visited[end] = true;
  q.push(end);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto& v : g.getInboundEdges(t)) {
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

// 3. Write a program that finds the connected components of an undirected graph 
// using a depth-first traversal of the graph. 
std::vector<std::unordered_set<int>> getConnectedComponentsDFS(graph::UndirectedGraph<int> g) {
  std::vector<std::unordered_set<int>> components;
  int component_index = 0;
  std::unordered_map<int, bool> visited;
  for (const auto& v : g) {
    if (visited[v])
      continue;
    std::stack<int> stk;
    stk.push(v);
    visited[v] = true;
    components.push_back(std::unordered_set<int>());
    components[component_index].insert(v);
    while (!stk.empty()) {
      int top = stk.top();
      stk.pop();
      for (const auto& adj : g.getAdjacentVertices(top)) {
        if (!visited[adj]) {
          stk.push(adj);
          visited[adj] = true;
          components[component_index].insert(adj);
        }
      }
    }
    ++component_index;
  }
  return components; 
}

void getGraphFromFile(std::string path, graph::UndirectedGraph<int>& g) {
  std::ifstream fin (path); 
  int nrOfVertices, nrOfEdges;
  fin >> nrOfVertices >> nrOfEdges;
  for (int v : std::views::iota(0, nrOfVertices)) {
    g.addVertex(v);
  }
  for (const auto& _ : std::views::iota(0, nrOfEdges)) {
    int from, to, c;
    fin >> from >> to >> c;
    try {
      g.addEdge(from, to);
    } catch (std::runtime_error& _) {
      continue;
    }
  }
}

int main() {
  graph::UndirectedGraph<int> g;
  getGraphFromFile("../input/graph1k.txt", g);

  int index = 0;
  for (const auto& component : getConnectedComponentsDFS(g)) {
    std::cout << "Component " << index++ << '\n';
    int i2 = 1;
    for (const auto& v : component) {
      std::cout << v << " ";
      if (i2++ % 40 == 0) {
	std::cout << '\n';
      }
    }
    std::cout << '\n';
  }
	return 0;
}
