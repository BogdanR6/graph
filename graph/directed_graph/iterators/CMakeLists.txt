add_library(directed_graph_iterators_lib Iterators.cpp)

target_include_directories(directed_graph_iterators_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
