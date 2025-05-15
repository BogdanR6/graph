#pragma once

#include "../DirectedGraph.hpp"

namespace graph {
namespace special {

struct Activity {
  int id;
  int duration;
  int earliestStart = 0;
  int latestStart = 0;
  std::string name = "";

  bool operator==(const Activity &other) const {
    return id == other.id;
  }
};
} // namespace special
} // namespace graph

namespace std {
template <>
struct hash<graph::special::Activity> {
  std::size_t operator()(const graph::special::Activity &a) const noexcept {
    return std::hash<int>{}(a.id);
  }
};
}

namespace graph {
namespace special {

class ActivityGraph : public DirectedGraph<Activity> {
public:
  GraphType getGraphType() const override;

  //must run first // TODO: consider auto running it with caching
  bool computeSchedule(); // returns false if cycle
  //
  int getTotalProjectTime() const;
  std::vector<Activity> getCriticalActivities() const;
  int getEarliestStart(const int& activityId) const;
  int getLatestStart(const int& activityId) const;
private:
  std::vector<Activity> sortedOrder;
  int totalProjectTime = 0;
};

} // namespace special
} // namespace graph
//
