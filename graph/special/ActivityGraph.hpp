#pragma once

#include "../directed_graph/DirectedGraph.hpp"
#include "../vertices/ActivityVertex.hpp"


namespace graph {
namespace special {

class ActivityGraph : public DirectedGraph {
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
