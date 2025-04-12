#include "../graph/DirectedGraph.hpp"
#include <stack>
#include <vector>
/*
 1. Write a program that, given a list of activities with duration and list of prerequisites for each activity,
    does the following:

    -> verify if the corresponding graph is a DAG and performs a topological sorting of the activities using the 
      algorithm based on depth-first traversal (Tarjan's algorithm);
    -> prints the earliest and the latest starting time for each activity and the total time of the project.
    -> prints the critical activities. 

*/
bool isCyclic(const graph::DirectedGraph<int> &graph) {
  // map each vertex to an index
  std::unordered_map<int, int> index;
  { // manipulate lifetime of i
    int i = 0;
    for (const auto& v : graph)
      index.insert({v, i++});
  }

  std::vector<bool> visited(graph.getNrOfVertices(), false);

  for (const auto& start : graph) {
    if (visited[index[start]]) 
      continue;
    // DFS
    std::vector<bool> inPath(graph.getNrOfVertices(), false);
    std::stack<std::pair<int, int>> stk;
    stk.push({start, 0}); // {vertex, state}
    while (!stk.empty()) {
      const auto &[v, state] = stk.top(); 
      stk.pop();


      if (state == 1) { // exit state
        inPath[index[v]] = false;
        continue;
      }
      
      visited[index[v]] = true;
      inPath[index[v]] = true;

      stk.push({v, 1}); // for exiting

      for (const auto& n : graph.getOutboundEdges(v)) {
        if (!visited[index[n]])
          stk.push({n, 0});
        else if (inPath[index[n]])
            return true;
      }

    }
  }
  return false;
}

/*
2. Write a program that, given a list of activities with duration and list of prerequisites for each activity, 
   does the following:

    -> verify if the corresponding graph is a DAG and performs a topological sorting of the activities 
      using the algorithm based on predecessor counters;
    -> prints the earliest and the latest starting time for each activity and the total time of the project.
    -> prints the critical activities. 

*/


/*
3. Write a program that, given a graph with costs, does the following:

    -> verify if the corresponding graph is a DAG and performs a topological sorting of the activities using the algorithm based on depth-first traversal (Tarjan's algorithm);
    -> if it is a DAG, finds a highest cost path between two given vertices, in O(m+n). 

*/


/*
4. Write a program that, given a graph with costs, does the following:

    -> verify if the corresponding graph is a DAG and performs a topological sorting of the activities using the algorithm based on predecessor counters;
    -> if it is a DAG, finds a highest cost path between two given vertices, in O(m+n). 

*/


/*
5. Write a program that, given an undirected connected graph, 
  constructs a minumal spanning tree using the Kruskal's algorithm.

*/


/*
6. Write a program that, given an undirected connected graph, 
  constructs a minumal spanning tree using the Prim's algorithm. 
*/
