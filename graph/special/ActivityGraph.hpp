#pragma once
#include "../directed_graph/DirectedGraph.hpp"


namespace graph {
namespace special {

class ActivityGraph : public DirectedGraph {
public:
  GraphType getGraphType() const override;

  //must run first // TODO: consider auto running it with caching
  bool computeSchedule(); // returns false if cycle
  //
  int getTotalProjectTime() const;
  std::vector<idT> getCriticalActivities() const;
  int getEarliestStart(const idT& activityId) const;
  int getLatestStart(const idT& activityId) const;
private:
  std::vector<idT> sortedOrder;
  int totalProjectTime = 0;
};

} // namespace special
} // namespace graph
//
