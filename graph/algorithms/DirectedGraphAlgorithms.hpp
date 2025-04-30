#include "../DirectedGraph.hpp"

namespace graph {
namespace algorithms {

int lowestLengthFBfs(graph::DirectedGraph<std::string> &g, std::string start, std::string end); 

int lowestLengthBBfs(graph::DirectedGraph<int> &g, std::string start, std::string end);

// 3.6 Computes the minimum cost walk between two vertices using matrix multiplication algorithm.
// Detects negative cost cycles.
std::pair<std::vector<std::string>, int> getLowestCostWalk(const graph::DirectedGraph<std::string> &g, const std::string &start, const std::string &end);
}
}
