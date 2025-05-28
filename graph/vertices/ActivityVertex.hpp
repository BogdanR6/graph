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
  void setDuration(int newDuration) { duration = newDuration; }

  int getEarliestStart() const { return earliestStart; }
  void setEarliestStart(int newEarliestStart) { earliestStart = newEarliestStart; }

  int getEarliestEnd() const { return earliestEnd; }
  void setEarliestEnd(int newEarliestEnd) { earliestEnd = newEarliestEnd; }
    
  int getLatestStart() const { return latestStart; }
  void setLatestStart(int newLatestStart) { latestStart = newLatestStart; }

  int getLatestEnd() const { return latestEnd; }
  void setLatestEnd(int newLatestEnd) { latestEnd = newLatestEnd; }

  std::string toString() const override {
    std::string displayableName = name.empty() ? "" : std::format(" name: {}", name);
    return std::format("{}{} (duration: {}, earliestStart: {}, latestStart: {}, earliestEnd: {}, latestEnd: {})",
                       id,
                       displayableName,
                       duration,
                       earliestStart,
                       latestStart,
                       earliestEnd,
                       latestEnd
                       );
    }
};

} // namespace special
} // namespace graph
