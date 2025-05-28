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


std::vector<idT> getMinimumVertexCover(const UndirectedGraph& graph) {
  std::vector<idT> vertices;
  for (const auto& [id, _] : graph) {
    vertices.push_back(id);
  }

  int n = vertices.size();
  std::vector<idT> bestCover;

  for (int mask = 0; mask < (1 << n); ++mask) {
    std::unordered_set<idT> cover;
    for (int i = 0; i < n; ++i) {
      if (mask & (1 << i)) {
        cover.insert(vertices[i]);
      }
    }

    // Check if this subset covers all edges
    bool allCovered = true;
    for (const Edge& edge : graph.getEdges()) {
      if (cover.count(edge.fromId) == 0 && cover.count(edge.toId) == 0) {
        allCovered = false;
        break;
      }
    }

    if (allCovered) {
      if (bestCover.empty() || cover.size() < bestCover.size()) {
        bestCover = std::vector<idT>(cover.begin(), cover.end());
      }
    }
  }

  return bestCover;
}

}//namespace algorightm
}// namespace graph
