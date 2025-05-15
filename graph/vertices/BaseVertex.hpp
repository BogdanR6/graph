#pragma once
#include <string>

class BaseVertex {
public:
  virtual std::string getId() const = 0;
  virtual ~BaseVertex() = default;
};
