#include "ActivityGraph.hpp"
#include "../abstract/Graph.hpp"
#include "../vertices/ActivityVertex.hpp"
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

  for (const auto& [activityId, _] : *this) {
    inDegree[activityId] = this->getAllInboundVertices(activityId).size();
  }

  for (const auto& [activityId, _] : *this) {
    if (inDegree[activityId] == 0) {
      queue.push(activityId);
    }
  }

  sortedOrder.clear();
  while (!queue.empty()) {
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(this->getVertex(queue.front())); 
    queue.pop();
    sortedOrder.push_back(activity->getId());
    for (auto& nextActivityId : this->getAllOutboundVertices(activity->getId())) {
      const std::shared_ptr<Activity> nextActivity = std::dynamic_pointer_cast<Activity>(this->getVertex(nextActivityId));
      inDegree[nextActivity->getId()]--;
      nextActivity->setEarliestStart(
        std::max(nextActivity->getEarliestStart(), activity->getEarliestStart() + activity->getDuration())
      );
      if (inDegree[nextActivity->getId()] == 0) {
        queue.push(nextActivity->getId());
      }
    }
  }

  if (sortedOrder.size() != this->getNrOfVertices())
    return false; // Cycle detected

  // Total project time is the max finish time among all activities
  int maxTime = 0;
  for (const auto& [_, vertex] : *this) {
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(vertex);
    maxTime = std::max(maxTime, activity->getEarliestStart() + activity->getDuration());
  }
  totalProjectTime = maxTime;

  // Compute latestStart times in reverse topological order
  for (auto it = sortedOrder.rbegin(); it != sortedOrder.rend(); ++it) {
    idT activityId = *it;
    const std::shared_ptr<Activity> activity = std::dynamic_pointer_cast<Activity>(getVertex(activityId));
    if (this->getAllOutboundVertices(activityId).empty()) {
      activity->setLatestStart(activity->getEarliestStart()); // on critical path
    } else {
      int minStart = std::numeric_limits<int>::max();
      for (const auto& nextActivityId : this->getAllOutboundVertices(activityId)) {
        const std::shared_ptr<Activity> nextActivity = std::dynamic_pointer_cast<Activity>(getVertex(nextActivityId));
        minStart = std::min(minStart, nextActivity->getLatestStart() - activity->getDuration());
      }
      activity->setLatestStart(minStart);
    }
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
