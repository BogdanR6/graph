#include "../undirected_graph/UndirectedGraph.hpp"
#include "UndirectedGraphAlgorithms.hpp"
#include <vector>
#include <stack>

namespace graph {
namespace algorithms {

// 2.3. Write a program that finds the connected components of an undirected graph
// using a depth-first traversal of the graph.
std::vector<graph::UndirectedGraph> getConnectedComponentsDFS(const graph::UndirectedGraph &g) {
  std::vector<graph::UndirectedGraph> components;

  std::unordered_map<std::string, bool> visited;

  for (const auto &[vertexId, vertex] : g) {
    if (visited[vertexId])
      continue;

    std::stack<std::string> stk;
    stk.push(vertexId);
    components.push_back(graph::UndirectedGraph());
    components.back().addVertex(vertex);
    visited[vertexId] = true;
    while (!stk.empty()) {
      std::string currentVertexId = stk.top();
      stk.pop();
      for (const auto &[_, adjId, __] : g.getAdjacentEdges(currentVertexId)) {
        if (!visited[adjId]) {
          stk.push(adjId);
          visited[adjId] = true;
          if (!components.back().isVertex(adjId))
            components.back().addVertex(g.getVertex(adjId));
          components.back().addEdge(currentVertexId, adjId);
        }
      }
    }
  }
  return components;
}
}//namespace algorightm
}// namespace graph
