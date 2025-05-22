#pragma once
#include <string>

namespace graph {

using idT = std::string;

class BaseVertex {
public:
  virtual idT getId() const = 0;
  //virtual std::string operator+(const BaseVertex &other) const = 0;
  virtual ~BaseVertex() = default;

  virtual std::string toString() const = 0;
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

}
