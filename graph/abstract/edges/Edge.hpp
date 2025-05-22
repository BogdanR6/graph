#pragma once
#include <string>

namespace graph {

using idT = std::string;

struct Edge {
  idT fromId;
  idT toId;
  int weight;

  Edge(const idT &from, const idT &to, int weight = 1)
      : fromId(from), toId(to), weight(weight) {}

  bool operator==(const Edge &other) const {
    return fromId == other.fromId && toId == other.toId;
  }
};

struct EdgeHash {
  std::size_t operator()(const Edge &e) const {
    return std::hash<idT>()(e.fromId) ^ (std::hash<idT>()(e.toId) << 1);
  }
};
}
