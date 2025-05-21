#include "../UndirectedGraph.hpp"

namespace graph {

class AdjacentEdgesView {
private:
    std::vector<Edge> edges;

public:
    AdjacentEdgesView(const UndirectedGraph &graph, const idT &fromId) {
        // Find the vertex in the adjacency map
        auto it = graph.adjacency.find(fromId);
        if (it != graph.adjacency.end()) {
            // Populate edges vector
            for (const idT& toId : it->second) {
                edges.push_back(Edge{fromId, toId, graph.getEdgeWeight(fromId, toId)});
            }
        }
    }

    // Standard container interface
    auto begin() const { return edges.begin(); }
    auto end() const { return edges.end(); }
    bool empty() const { return edges.empty(); }
};

} // namespace graph
