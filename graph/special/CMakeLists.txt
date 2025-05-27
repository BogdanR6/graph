add_library(activity_graph_lib ActivityGraph.cpp)
target_include_directories(activity_graph_lib
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
