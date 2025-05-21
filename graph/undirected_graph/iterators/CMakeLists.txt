add_library(undirected_graph_iterators_lib Iterators.cpp)

target_include_directories(undirected_graph_iterators_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
