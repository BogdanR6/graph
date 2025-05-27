#pragma once
#include "BaseVertex.hpp"
#include <string>
#include <format>


namespace graph {
namespace special {

struct Activity : public BaseVertex {
  idT id;
  std::string name = "";
  int duration;
  int earliestStart = 0;
  int latestStart = 0;

  idT getId() const override {
    return id;
  }

  std::string toString() const override {
    std::string displayableName = name.empty() ? "" : std::format(" name: {}", name);
    return std::format("{}{} (duration: {}, earliest: {}, latest: {})",
                       id,
                       displayableName,
                       duration,
                       earliestStart,
                       latestStart
                       );
    }
};

} // namespace special
} // namespace graph
