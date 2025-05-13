#include "../DirectedGraph.hpp"

namespace graph {
namespace algorithms {

int lowestLengthFBfs(graph::DirectedGraph<std::string> &g, std::string start, std::string end); 

int lowestLengthBBfs(graph::DirectedGraph<int> &g, std::string start, std::string end);

std::pair<std::vector<std::string>, int> getLowestCostWalk(const graph::DirectedGraph<std::string> &g, const std::string &start, const std::string &end);
std::vector<std::string> getTopologicalOrder(const graph::DirectedGraph<std::string> &g);
}
}
