#pragma once
#include "../edges/Edge.hpp"
#include <vector>

namespace graph {

class AdjacentEdgesView {
private:
    std::vector<Edge> edges;
public:
    void addEdge(const Edge & edge) { edges.push_back(edge); }
    auto begin() const { return edges.begin(); }
    auto end() const { return edges.end(); }
    bool empty() const { return edges.empty(); }
    std::vector<Edge> getAll() { return edges; }
};

} // namespace graph
