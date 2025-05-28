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
  int earliestEnd = 0;
  int latestStart = 0;
  int latestEnd = 0;

public:
  explicit Activity(idT id, std::string name, int duration) : id(id), name(name), duration(duration) {}
  idT getId() const override {
    return id;
  }

  std::string getName() const { return name; }
  void setName(const std::string &newName) { name = newName; }

  int getDuration() const { return duration; }
  void setDuration(int newDuration) { name = newDuration; }

  int getEarliestStart() const { return earliestStart; }
  void setEarliestStart(int newEarliestStart) { name = newEarliestStart; }

  int getEarliestEnd() const { return earliestEnd; }
  void setEarliestEnd(int newEarliestEnd) { name = newEarliestEnd; }
    
  int getLatestStart() const { return latestStart; }
  void setLatestStart(int newLatestStart) { name = newLatestStart; }

  int getLatestEnd() const { return latestEnd; }
  void setLatestEnd(int newLatestEnd) { name = newLatestEnd; }

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
