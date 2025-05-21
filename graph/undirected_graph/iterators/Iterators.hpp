#include "../UndirectedGraph.hpp"

namespace graph {

class AdjacentEdgesView {
private:
    std::vector<Edge> edges;

public:
    AdjacentEdgesView(const UndirectedGraph& graph, const VertexSharedPtr& vertex) {
        // Find the vertex in the adjacency map
        auto it = graph.adjacency.find(vertex);
        if (it != graph.adjacency.end()) {
            // Populate edges vector
            for (const VertexSharedPtr& to : it->second) {
                edges.push_back(Edge{vertex, to, graph.getEdgeWeight(vertex, to)});
            }
        }
    }

    // Standard container interface
    auto begin() const { return edges.begin(); }
    auto end() const { return edges.end(); }
    bool empty() const { return edges.empty(); }
};

} // namespace graph
