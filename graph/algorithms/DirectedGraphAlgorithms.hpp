#include "../directed_graph/DirectedGraph.hpp"

namespace graph {
namespace algorithms {

int lowestLengthFBfs(graph::DirectedGraph &g, const idT &startId, const idT &endId); 

int lowestLengthBBfs(graph::DirectedGraph &g, const idT &startId, const idT &endId);

std::pair<std::vector<idT>, int> getLowestCostWalk(const graph::DirectedGraph &g, const idT &startId, const idT &endId);
std::vector<idT> getTopologicalOrder(const graph::DirectedGraph &g);
}
}
