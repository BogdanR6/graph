#include "../UndirectedGraph.hpp"
#include "UndirectedGraphAlgorithms.hpp"
#include <vector>
#include <stack>

namespace graph {
namespace algorithms {

// 3. Write a program that finds the connected components of an undirected graph
// using a depth-first traversal of the graph.
std::vector<graph::UndirectedGraph<std::string>>
getConnectedComponentsDFS(const graph::UndirectedGraph<std::string> &g) {
  std::vector<graph::UndirectedGraph<std::string>> components;

  std::unordered_map<std::string, bool> visited;

  for (const auto &v : g) {
    if (visited[v])
      continue;

    std::stack<std::string> stk;
    stk.push(v);
    components.push_back(graph::UndirectedGraph<std::string>());
    components.back().addVertex(v);
    visited[v] = true;
    while (!stk.empty()) {
      std::string top = stk.top();
      stk.pop();
      for (const auto &[_, adj, __] : g.getAdjacentEdges(top)) {
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
}
}
