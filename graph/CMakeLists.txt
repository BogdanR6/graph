add_library(
  graph_lib STATIC
  algorithms/UndirectedGraphAlgorithms.cpp
  algorithms/DirectedGraphAlgorithms.cpp special/ActivityGraph.cpp)

target_include_directories(
  graph_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/special
         ${CMAKE_CURRENT_SOURCE_DIR}/vertices)
