#include "CommandController.hpp"
#include "../errors/InvalidInputError.cpp"

CommandController::CommandController(Console& console, GraphService& graphService)
  : graphService(graphService) {

  // Register the commands
  console.registerCommand("exit", [&](const auto& args) -> CommandResult {
    return {"Exiting", true};
  });

  console.registerCommand("add_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: add_vertex <vertex_id>");

    TElem vertexId = args[1];
    graphService.addVertex(vertexId);
    return {"Vertex added."};
  });

  console.registerCommand("remove_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: remove_vertex <vertex_id>");

    TElem vertexId = args[1];
    graphService.removeVertex(vertexId);
    return {"Vertex removed."};
  });

  console.registerCommand("is_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: is_vertex <vertex_id>");

    TElem vertexId = args[1];
    return {"Vertex " + vertexId + (graphService.isVertex(vertexId) ? " is " : " is NOT ") + "in the graph"};
  });

  console.registerCommand("add_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3 && args.size() != 4)
      throw InvalidUsageError("Usage: add_edge <from_vertex_id> <to_vertex_id> [weight = 1]");

    TElem fromVertexId = args[1];
    TElem toVertexId = args[2];
    int weight = 1; 
    if (args.size() == 4)
      weight = std::stoi(args[3]); 

    graphService.addEdge(fromVertexId, toVertexId, weight);
    return {"Edge added."};
  });

  console.registerCommand("remove_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: remove_edge <from_vertex_id> <to_vertex_id>");

    TElem fromVertexId = args[1];
    TElem toVertexId = args[2];

    graphService.removeEdge(fromVertexId, toVertexId);
    return {"Edge removed."};
  });


  console.registerCommand("is_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: is_edge <from_vertex_id> <to_vertex_id>");

    TElem fromVertexId = args[1];
    TElem toVertexId = args[2];

    
    return {"Edge " + fromVertexId + "->" + toVertexId + 
            (graphService.isEdge(fromVertexId, toVertexId) ? " is " : " is NOT ") + "in the graph"};
  });


  console.registerCommand("list_vertices", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_vertices");

    return {graphService.getVertices()};
  });

  console.registerCommand("list_adj", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: list_adj <vertex_id>");

    TElem vertexId = args[1];
    return {graphService.getAdjacentVertices(vertexId)};
  });
  

  console.registerCommand("list_edges", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_edges");

    return {graphService.getEdges()};
  });
}
