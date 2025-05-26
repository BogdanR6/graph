add_library(directed_graph_algorithms_lib DirectedGraphAlgorithms.cpp)
target_include_directories(directed_graph_algorithms_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(undirected_graph_algorithms_lib UndirectedGraphAlgorithms.cpp)
target_include_directories(undirected_graph_algorithms_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
