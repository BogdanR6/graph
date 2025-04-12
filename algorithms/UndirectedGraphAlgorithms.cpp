#include "../graph/UndirectedGraph.hpp"
#include <vector>
#include <stack>

// 3. Write a program that finds the connected components of an undirected graph
// using a depth-first traversal of the graph.
std::vector<graph::UndirectedGraph<int>>
getConnectedComponentsDFS(const graph::UndirectedGraph<int> &g) {
  std::vector<graph::UndirectedGraph<int>> components;

  std::unordered_map<int, bool> visited;

  for (const auto &v : g) {
    if (visited[v])
      continue;

    std::stack<int> stk;
    stk.push(v);
    components.push_back(graph::UndirectedGraph<int>());
    components.back().addVertex(v);
    visited[v] = true;
    while (!stk.empty()) {
      int top = stk.top();
      stk.pop();
      for (const auto &adj : g.getAdjacentVertices(top)) {
        if (!visited[adj]) {
          stk.push(adj);
          visited[adj] = true;
          if (!components.back().isVertex(adj))
            components.back().addVertex(adj);
          components.back().addEdge(top, adj);
        }
      }
    }
  }
  return components;
}

