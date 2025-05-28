#include "../undirected_graph/UndirectedGraph.hpp"

namespace graph {
namespace algorithms {

// 3. Write a program that finds the connected components of an undirected graph
// using a depth-first traversal of the graph.
std::vector<graph::UndirectedGraph> getConnectedComponentsDFS(const UndirectedGraph &g);

std::vector<idT> getMinimumVertexCover(const UndirectedGraph& graph);
}
}
