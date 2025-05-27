#pragma once
#include "BaseVertex.hpp"
#include <string>
#include <format>


namespace graph {
namespace special {

class Activity : public BaseVertex {
  idT id;
  std::string name = "";
  int duration;
  int earliestStart = 0;
  int latestStart = 0;

public:
  idT getId() const override {
    return id;
  }

  std::string getName() const { return name; }
  void setName(const std::string &newName) { name = newName; }

  int getDuration() const { return duration; }
  void setDuration(int newDuration) { name = newDuration; }

  int getEarliestStart() const { return duration; }
  void setEarliestStart(int newEarliestStart) { name = newEarliestStart; }

  int getLatestStart() const { return earliestStart; }
  void setLatestStart(int newLatestStart) { name = newLatestStart; }

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
