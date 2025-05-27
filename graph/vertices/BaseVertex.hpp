#pragma once
#include <string>
#include <memory>

namespace graph {

using idT = std::string;

class BaseVertex {
public:
  virtual idT getId() const = 0;
  virtual ~BaseVertex() = default;

  [[ nodiscard ]] virtual std::string toString() const = 0;

  bool operator==(const BaseVertex &other) const {
    return getId() == other.getId();
  }
};

inline std::string operator+(const std::string &lhs, const BaseVertex &rhs) {
    return lhs + rhs.getId();
}

inline std::string operator+(const BaseVertex &lhs, const std::string &rhs) {
    return lhs.getId() + rhs;
}

inline std::ostream &operator<<(std::ostream &os, const BaseVertex &vertex) {
    os << vertex.getId();
    return os;
}

struct VertexPtrEqual {
    bool operator()(const std::shared_ptr<BaseVertex>& lhs,
                    const std::shared_ptr<BaseVertex>& rhs) const {
        return lhs && rhs && lhs->getId() == rhs->getId();
    }
};

struct VertexPtrHash {
    std::size_t operator()(const std::shared_ptr<BaseVertex>& v) const {
        return std::hash<graph::idT>{}(v->getId());
    }
};

} // namespace graph
