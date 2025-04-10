#include "interface/directedGraph.hpp"
#include "interface/undirectedGraph.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <stack>

#if defined(__linux__) // Or #if __linux__
#define CLEAR_SCREEN system("clear");
#elif _WIN32
#define CLEAR_SCREEN system("cls");
#else
#define CLEAR_SCREEN system("clear");
#endif

int lowestLengthFBfs(graph::DirectedGraph<int> &g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int>
  distance; // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[start] = 0;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto &v : g.getOutboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (v == end)
        return distance[t] + 1;

      if (!visited[v]) {
        visited[v] = true;
        distance[v] = distance[t] + 1;
        q.push(v);
      }
    }
  }

  return 999; // inf
}

int lowestLengthBBfs(graph::DirectedGraph<int> &g, int start, int end) {
  std::unordered_map<int, bool> visited;
  std::unordered_map<int, int>
      distance; // Stores the shortest distance from start to each node
  std::queue<int> q;

  distance[end] = 0;
  visited[end] = true;
  q.push(end);
  while (!q.empty()) {
    int t = q.front();
    q.pop();

    for (const auto &v : g.getInboundEdges(t)) {
      // If we have reached the end node, return the distance
      if (v == start)
        return distance[t] + 1;

      if (!visited[v]) {
        visited[v] = true;
        distance[v] = distance[t] + 1;
        q.push(v);
      }
    }
  }

  return 999; // inf
}

// 3. Write a program that finds the connected components of an undirected graph
// using a depth-first traversal of the graph.
std::vector<graph::UndirectedGraph<int>>
getConnectedComponentsDFS(const graph::UndirectedGraph<int> &g) {
  std::vector<graph::UndirectedGraph<int>> components;

  std::unordered_map<int, bool> visited;

  for (const auto &v : g) {
    if (visited[v])
      continue;

    std::stack<int> stk;
    stk.push(v);
    components.push_back(graph::UndirectedGraph<int>());
    components.back().addVertex(v);
    visited[v] = true;
    while (!stk.empty()) {
      int top = stk.top();
      stk.pop();
      for (const auto &adj : g.getAdjacentVertices(top)) {
        if (!visited[adj]) {
          stk.push(adj);
          visited[adj] = true;
          if (!components.back().isVertex(adj))
            components.back().addVertex(adj);
          components.back().addEdge(top, adj);
        }
      }
    }
  }
  return components;
}

graph::UndirectedGraph<int> getGraphFromFile(std::string path) {
  graph::UndirectedGraph<int> g;
  std::ifstream fin(path);
  int nrOfVertices, nrOfEdges;
  fin >> nrOfVertices >> nrOfEdges;
  for (int v : std::views::iota(0, nrOfVertices)) {
    g.addVertex(v);
  }
  for (const auto &_ : std::views::iota(0, nrOfEdges)) {
    int from, to, c;
    fin >> from >> to >> c;
    try {
      g.addEdge(from, to);
    } catch (std::runtime_error &_) {
      continue;
    }
  }
  return g;
}

void printMenu() {
  // CLEAR_SCREEN
  std::cout << "\n--- Graph Menu ---\n";
  std::cout << "1. Add vertex\n";
  std::cout << "2. Remove vertex\n";
  std::cout << "3. Add edge\n";
  std::cout << "4. Remove edge\n";
  std::cout << "5. Check if vertex exists\n";
  std::cout << "6. Check if edge exists\n";
  std::cout << "7. List all vertices\n";
  std::cout << "8. Show adjacent vertices\n";
  std::cout << "9. Load Graph from file\n";
  std::cout << "10. Get connected components\n";
  // std::cout << "11. Show adjacent vertices\n";
  // std::cout << "12. Show adjacent vertices\n";
  std::cout << "0. Exit\n";
  std::cout << "Enter your choice: ";
}

// for next time problem 6
int main() {
  graph::UndirectedGraph<int> g;
  int choice;
  int v1, v2;

  while (true) {
    printMenu();
    std::cin >> choice;

    switch (choice) {
    case 1:
      std::cout << "Enter vertex to add: ";
      std::cin >> v1;
      g.addVertex(v1);
      std::cout << "Vertex added.\n";
      break;
    case 2:
      std::cout << "Enter vertex to remove: ";
      std::cin >> v1;
      g.removeVertex(v1);
      std::cout << "Vertex removed.\n";
      break;
    case 3:
      std::cout << "Enter two vertices to connect (edge): ";
      std::cin >> v1 >> v2;
      g.addEdge(v1, v2);
      std::cout << "Edge added.\n";
      break;
    case 4:
      std::cout << "Enter two vertices to disconnect (remove edge): ";
      std::cin >> v1 >> v2;
      g.removeEdge(v1, v2);
      std::cout << "Edge removed.\n";
      break;
    case 5:
      std::cout << "Enter vertex to check: ";
      std::cin >> v1;
      std::cout << (g.isVertex(v1) ? "Vertex exists.\n"
                                   : "Vertex does not exist.\n");
      break;
    case 6:
      std::cout << "Enter two vertices to check for edge: ";
      std::cin >> v1 >> v2;
      std::cout << (g.isEdge(v1, v2) ? "Edge exists.\n"
                                     : "Edge does not exist.\n");
      break;
    case 7: {
      std::cout << "Vertices in the graph:\n";
      int i = 1;
      for (auto it = g.begin(); it != g.end(); ++it) {
        std::cout << *it << " ";
        if (i++ % 30 == 0)
          std::cout << '\n';
      }
      std::cout << '\n';
      std::cout << "Nr of vertices: " << g.getNrOfVertices() << '\n';
      std::cout << "Nr of edges:    " << g.getNrOfEdges() << '\n';
      break;
    }
    case 8:
      std::cout << "Enter vertex: ";
      std::cin >> v1;
      if (!g.isVertex(v1)) {
        std::cout << "Vertex does not exist.\n";
        break;
      }
      std::cout << "Adjacent vertices to " << v1 << ":\n";
      for (const auto &v : g.getAdjacentVertices(v1)) {
        std::cout << v << "\n";
      }
      break;
    case 9: {
      std::cout << "Enter input name: ";
      std::string fileName;
      std::cin >> fileName;
      std::string filePath = "../input/" + fileName;
      g = getGraphFromFile(filePath);
      break;
    }
    case 10: {
      int i = 1;
      for (const auto &grph : getConnectedComponentsDFS(g)) {
        std::cout << "Component " << i++ << '\n';
        int i2 = 1;
        for (const auto &v : grph) {
          std::cout << v << ' ';
          if (i2++ % 30 == 0)
            std::cout << '\n';
        }
        std::cout << '\n';
      }
      break;
    }
    case 11:
      std::cout << "Not implemented\n";
      break;
    case 12:
      std::cout << "Not implemented\n";
      break;
    case 0:
      std::cout << "Exiting...\n";
      return 0;
    default:
      std::cout << "Invalid option. Try again.\n";
      break;
    }
  }

  return 0;
}
