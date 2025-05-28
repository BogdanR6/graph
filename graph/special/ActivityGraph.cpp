#include "ActivityGraph.hpp"
#include "../abstract/Graph.hpp"
#include "../vertices/ActivityVertex.hpp"
#include <algorithm>
#include <memory>
#include <queue>
#include <limits>

namespace graph {
namespace special {

GraphType ActivityGraph::getGraphType() const {
  return GraphType::Activity;
}

bool ActivityGraph::computeSchedule() {
  // Topological sort using predecessor counters
  std::unordered_map<idT, int> inDegree;
  std::queue<idT> queue;

  // Compute in degree for topological sorting
  for (const auto& [activityId, _] : *this) {
    inDegree[activityId] = this->getAllInboundVertices(activityId).size();
  }

  // Add the vertices with no inbound edges to the queue
  for (const auto& [activityId, _] : *this) {
    if (inDegree[activityId] == 0) {
      queue.push(activityId);
    }
  }

  // Compute topological order and earliest start/end times (forward traversal)
  sortedOrder.clear();
  while (!queue.empty()) {
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(this->getVertex(queue.front())); 
    queue.pop();
    sortedOrder.push_back(activity->getId());

    activity->setEarliestEnd(activity->getEarliestStart() + activity->getDuration());
    for (auto& nextActivityId : this->getAllOutboundVertices(activity->getId())) {
      const std::shared_ptr<Activity> nextActivity = std::dynamic_pointer_cast<Activity>(this->getVertex(nextActivityId));
      inDegree[nextActivity->getId()]--;

      int newEarliestStart = std::max(nextActivity->getEarliestStart(), activity->getEarliestEnd());
      nextActivity->setEarliestStart(newEarliestStart);
      nextActivity->setEarliestEnd(newEarliestStart + nextActivity->getDuration());
      if (inDegree[nextActivity->getId()] == 0) {
        queue.push(nextActivity->getId());
      }
    }
  }

  // Check for cycles
  if (sortedOrder.size() != this->getNrOfVertices())
    return false; // Cycle detected

  // Compute total project time (the max finish time among all activities)
  int maxTime = 0;
  for (const auto& [_, vertex] : *this) {
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(vertex);
    maxTime = std::max(maxTime, activity->getEarliestEnd());
  }
  totalProjectTime = maxTime;

  // Compute latest start/end times (reverse traversal)
  for (auto it = sortedOrder.rbegin(); it != sortedOrder.rend(); ++it) {
    auto activity = std::dynamic_pointer_cast<Activity>(getVertex(*it));

    if (this->getAllOutboundVertices(*it).empty()) {
      // Terminal activity: latestEnd = totalProjectTime
      activity->setLatestEnd(totalProjectTime);
    } else {
      int minEnd = std::numeric_limits<int>::max();
      for (const auto& nextId : this->getAllOutboundVertices(*it)) {
        auto nextActivity = std::dynamic_pointer_cast<Activity>(getVertex(nextId));
        minEnd = std::min(minEnd, nextActivity->getLatestStart());
      }
      activity->setLatestEnd(minEnd);
    }

    activity->setLatestStart(activity->getLatestEnd() - activity->getDuration());
  }

  return true;
}

int ActivityGraph::getTotalProjectTime() const {
  return totalProjectTime;
}

std::vector<idT> ActivityGraph::getCriticalActivities() const {
  std::vector<idT> result;
  for (const auto& [_, vertex] : *this) {
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(vertex);
    if (activity->getEarliestStart() == activity->getLatestStart()) {
      result.push_back(activity->getId());
    }
  }
  return result;
}

int ActivityGraph::getEarliestStart(const idT& activityId) const {
  return std::dynamic_pointer_cast<Activity>(this->getVertex({activityId}))->getEarliestStart();
}

int ActivityGraph::getLatestStart(const idT& activityId) const {
  return std::dynamic_pointer_cast<Activity>(this->getVertex({activityId}))->getLatestStart();
}

} // namespace special
} // namespace graph
