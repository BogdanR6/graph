#pragma once
#include "BaseVertex.hpp"

class StringVertex : public BaseVertex {
private:
  std::string id;
public:
  explicit StringVertex(const std::string &id) : id(id) {}

  std::string getId() const override {
    return id;
  }
};


