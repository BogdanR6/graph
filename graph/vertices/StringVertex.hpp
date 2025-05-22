#pragma once
#include "BaseVertex.hpp"

namespace graph {

class StringVertex : public BaseVertex {
private:
  std::string id;
public:
  explicit StringVertex(const std::string &id) : id(id) {}

  std::string getId() const override {
    return id;
  }

  std::string toString() const override {
    return id;
  }
};

}

