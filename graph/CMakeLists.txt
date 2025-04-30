add_library(graph_lib STATIC
  algorithms/UndirectedGraphAlgorithms.cpp
  algorithms/DirectedGraphAlgorithms.cpp
)

target_include_directories(graph_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
