#include "CommandController.hpp"
#include "../errors/InvalidInputError.cpp"

CommandController::CommandController(Console& console, GraphService& graphService)
  : graphService(graphService) {

  // Register the commands
  console.registerCommand("help", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("`help` command does not get arguments!");
    std::string commandsMan = "";
    for (const auto &[command, description] : console.getMan()) {
      commandsMan += command + " -> " + description + "\n";
    }
    return {commandsMan};
  });

  console.documentCommand("exit", "Exits the program");
  console.registerCommand("exit", [&](const auto& args) -> CommandResult {
    return {"Exiting", true};
  });

  console.documentCommand("add_vertex", "Adds vertex to the graph");
  console.registerCommand("add_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: add_vertex <vertex_id>");

    TElem vertexId = args[1];
    graphService.addVertex(vertexId);
    return {"Vertex added."};
  });

  console.documentCommand("remove_vertex", "Removes vertex from the graph");
  console.registerCommand("remove_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: remove_vertex <vertex_id>");

    TElem vertexId = args[1];
    graphService.removeVertex(vertexId);
    return {"Vertex removed."};
  });

  console.documentCommand("is_vertex", "Checks if the vertex is in the graph");
  console.registerCommand("is_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: is_vertex <vertex_id>");

    TElem vertexId = args[1];
    return {"Vertex " + vertexId + (graphService.isVertex(vertexId) ? " is " : " is NOT ") + "in the graph"};
  });

  console.documentCommand("add_edge", "Adds an edge between two existing vertices to the graph");
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

  console.documentCommand("remove_edge", "Removes edge from the graph");
  console.registerCommand("remove_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: remove_edge <from_vertex_id> <to_vertex_id>");

    TElem fromVertexId = args[1];
    TElem toVertexId = args[2];

    graphService.removeEdge(fromVertexId, toVertexId);
    return {"Edge removed."};
  });


  console.documentCommand("is_edge", "Checks if the edge is in the graph");
  console.registerCommand("is_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: is_edge <from_vertex_id> <to_vertex_id>");

    TElem fromVertexId = args[1];
    TElem toVertexId = args[2];

    
    return {"Edge " + fromVertexId + "->" + toVertexId + 
            (graphService.isEdge(fromVertexId, toVertexId) ? " is " : " is NOT ") + "in the graph"};
  });


  console.documentCommand("list_vertices", "Display all the vertices in the current graph");
  console.registerCommand("list_vertices", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_vertices");

    return {graphService.getVertices()};
  });

  console.documentCommand("list_adj", "Display all the vertices adjacent with the given vertex");
  console.registerCommand("list_adj", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: list_adj <vertex_id>");

    TElem vertexId = args[1];
    return {graphService.getAdjacentVertices(vertexId)};
  });
  

  console.documentCommand("list_edges", "Display all the edges in the graph");
  console.registerCommand("list_edges", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_edges");

    return {graphService.getEdges()};
  });

  console.documentCommand("load_graph", "Loads a graph from a file");
  console.registerCommand("load_graph", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: load_graph <file_path>");
    std::string path = args[1];
    return {graphService.loadGraph(path)};
  });
}
