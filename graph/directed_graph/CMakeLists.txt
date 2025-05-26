add_library(directed_graph_lib DirectedGraph.cpp)
target_include_directories(directed_graph_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
