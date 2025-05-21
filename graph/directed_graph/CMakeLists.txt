add_subdirectory(iterators)

add_library(directed_graph_lib DirectedGraph.cpp)
target_include_directories(directed_graph_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(directed_graph_lib PUBLIC directed_graph_iterators_lib)
