#pragma once
#include "../DirectedGraph.hpp"

namespace graph {

class OutboundEdgesIterator {
private:
  const DirectedGraph &graph;
  const idT &vertexId;

public:
  class Iterator {
  private:
    typename std::unordered_set<idT>::const_iterator it;
    const idT &fromId;
    const DirectedGraph &graph;

  public:
    Iterator(typename std::unordered_set<idT>::const_iterator it, const idT &fromId, const DirectedGraph &graph)
      : it(it), fromId(fromId), graph(graph) {}

    Edge operator*() const {
      return Edge{fromId, *it, graph.getEdgeWeight(fromId, *it)};
    }

    Iterator& operator++() {
      ++it;
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return it == other.it;
    }

    bool operator!=(const Iterator& other) const {
      return it != other.it;
    }
  };

  OutboundEdgesIterator(const DirectedGraph& graph, const idT& vertexId)
    : graph(graph), vertexId(vertexId) {}

  Iterator begin() const {
    return Iterator(graph.outAdjacency.at(vertexId).begin(), vertexId, graph);
  }

  Iterator end() const {
    return Iterator(graph.outAdjacency.at(vertexId).end(), vertexId, graph);
  }
};

class InboundEdgesIterator {
private:
  const DirectedGraph &graph; 
  const idT &vertexId;
public:
  class Iterator {
  private:
    typename std::unordered_set<idT>::const_iterator it;
    const idT &toId;
    const DirectedGraph &graph;

  public:
    Iterator(typename std::unordered_set<idT>::const_iterator it, const idT &toId, const DirectedGraph &graph)
      : it(it), toId(toId), graph(graph) {}

    Edge operator*() const {
      return Edge{*it, toId, graph.getEdgeWeight(*it, toId)};
    }

    Iterator& operator++() {
      ++it;
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return it == other.it;
    }

    bool operator!=(const Iterator& other) const {
      return it != other.it;
    }
  };

  InboundEdgesIterator(const DirectedGraph &graph, const idT &vertexId)
    : graph(graph), vertexId(vertexId) {}

  Iterator begin() const {
    return Iterator(graph.inAdjacency.at(vertexId).begin(), vertexId, graph);
  }

  Iterator end() const {
    return Iterator(graph.inAdjacency.at(vertexId).end(), vertexId, graph);
  }
};

} // namespace graph
