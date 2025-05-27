// #include "graph/DirectedGraph.hpp"
// #include "graph/UndirectedGraph.hpp"
#include "ui/Console.hpp"
#include "controller/CommandController.hpp"
#include "service/GraphService.hpp"
#include <assert.h>
#include <iostream>


int main() {
  Console console; 
  GraphService service;
  CommandController controller(console, service);
  console.startConsoleLoop();
  return 0;
}
