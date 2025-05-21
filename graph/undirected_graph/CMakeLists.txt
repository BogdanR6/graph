add_subdirectory(iterators)

add_library(undirected_graph_lib UndirectedGraph.cpp)
target_include_directories(undirected_graph_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(undirected_graph_lib
                      PUBLIC undirected_graph_iterators_lib)
