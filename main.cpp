// #include "graph/DirectedGraph.hpp"
// #include "graph/UndirectedGraph.hpp"
#include "ui/Console.hpp"
#include "controller/CommandController.hpp"
#include "service/GraphService.hpp"
#include <assert.h>
#include <iostream>


void printMenu() {
  // CLEAR_SCREEN
  std::cout << "8. Show adjacent vertices\n";
  std::cout << "9. Load Graph from file\n";
  std::cout << "10. Get connected components\n";
  // std::cout << "11. Show adjacent vertices\n";
  // std::cout << "12. Show adjacent vertices\n";
  std::cout << "0. Exit\n";
  std::cout << "Enter your choice: ";
}


int main() {
  Console console; 
  GraphService service;
  CommandController controller(console, service);
  console.startConsoleLoop();
  return 0;
}
