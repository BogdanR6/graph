#pragma once
#include "ActivityGraph.hpp"
#include "../abstract/Graph.hpp"
#include <queue>
#include <limits>

namespace graph {
namespace special {

GraphType ActivityGraph::getGraphType() const {
  return GraphType::Activity;
}

bool ActivityGraph::computeSchedule() {
  // Topological sort using predecessor counters
  std::unordered_map<int, int> inDegree;
  std::queue<Activity> q;

  for (const auto& activity : *this) {
    inDegree[activity.id] = this->getAllInnoundVertices(activity).size();
  }

  for (const auto& activity : *this) {
    if (inDegree[activity.id] == 0) {
      q.push(activity);
    }
  }

  sortedOrder.clear();
  while (!q.empty()) {
    Activity activity = q.front(); q.pop();
    sortedOrder.push_back(activity);
    for (auto& nextActivity : this->getAllOutboundVertices(activity)) {
      Activity &modNextActivity = this->getVertex(nextActivity);
      inDegree[modNextActivity.id]--;
      modNextActivity.earliestStart = std::max(modNextActivity.earliestStart, activity.earliestStart + activity.duration);
      if (inDegree[modNextActivity.id] == 0) {
        q.push(modNextActivity);
      }
    }
  }

  if (sortedOrder.size() != this->getNrOfVertices())
    return false; // Cycle detected

  // Total project time is the max finish time among all activities
  int maxTime = 0;
  for (const auto& act : *this) {
    maxTime = std::max(maxTime, act.earliestStart + act.duration);
  }
  totalProjectTime = maxTime;

  // Compute latestStart times in reverse topological order
  for (auto it = sortedOrder.rbegin(); it != sortedOrder.rend(); ++it) {
    Activity& u = const_cast<Activity&>(*it);
    if (this->getAllOutboundVertices(u).empty()) {
      u.latestStart = u.earliestStart; // on critical path
    } else {
      int minStart = std::numeric_limits<int>::max();
      for (const auto& v : this->getAllOutboundVertices(u)) {
        minStart = std::min(minStart, v.latestStart - u.duration);
      }
      u.latestStart = minStart;
    }
  }

  return true;
}

int ActivityGraph::getTotalProjectTime() const {
  return totalProjectTime;
}

std::vector<Activity> ActivityGraph::getCriticalActivities() const {
  std::vector<Activity> result;
  for (const auto& act : *this) {
    if (act.earliestStart == act.latestStart) {
      result.push_back(act);
    }
  }
  return result;
}

int ActivityGraph::getEarliestStart(const int& activityId) const {
  return this->getVertex({activityId}).earliestStart;
}

int ActivityGraph::getLatestStart(const int& activityId) const {
  return this->getVertex({activityId}).latestStart;
}

} // namespace special
} // namespace graph
