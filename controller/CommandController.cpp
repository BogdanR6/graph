#include "CommandController.hpp"
#include "../errors/InvalidInputError.cpp"
#include "../graph/vertices/StringVertex.hpp"
#include <string>
#include <format>

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
    commandsMan = commandsMan.substr(0, commandsMan.length() - 1); // eliminate the last new line
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

    std::string vertexId = args[1];
    graph::StringVertex vertex(vertexId);
    graphService.addVertex(vertex);
    return {"Vertex added."};
  });

  console.documentCommand("remove_vertex", "Removes vertex from the graph");
  console.registerCommand("remove_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: remove_vertex <vertex_id>");

    std::string vertexId = args[1];
    graphService.removeVertex(vertexId);
    return {"Vertex removed."};
  });

  console.documentCommand("is_vertex", "Checks if the vertex is in the graph");
  console.registerCommand("is_vertex", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: is_vertex <vertex_id>");

    std::string vertexId = args[1];
    return {std::format("Vertex {} is {}in the graph.", vertexId, graphService.isVertex(vertexId) ? "" : "not ")};
  });

  console.documentCommand("add_edge", "Adds an edge between two existing vertices to the graph");
  console.registerCommand("add_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3 && args.size() != 4)
      throw InvalidUsageError("Usage: add_edge <from_vertex_id> <to_vertex_id> [weight = 1]");

    std::string fromVertexId = args[1];
    std::string toVertexId = args[2];
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

    std::string fromVertexId = args[1];
    std::string toVertexId = args[2];

    graphService.removeEdge(fromVertexId, toVertexId);
    return {"Edge removed."};
  });


  console.documentCommand("is_edge", "Checks if the edge is in the graph");
  console.registerCommand("is_edge", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: is_edge <from_vertex_id> <to_vertex_id>");

    std::string fromVertexId = args[1];
    std::string toVertexId = args[2];

    
    return {"Edge " + fromVertexId + "->" + toVertexId + 
            (graphService.isEdge(fromVertexId, toVertexId) ? " is " : " is NOT ") + "in the graph"};
  });


  console.documentCommand("list_vertices", "Display all the vertices in the current graph");
  console.registerCommand("list_vertices", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_vertices");
    std::vector<graph::VertexSharedPtr> vertices = graphService.getVertices();
    if (vertices.empty())
      return {"The graph contains no vertices!"};
    std::string output = (vertices.size() > 1 ? "The vertices in the graph are:\n" : "The vertex in the graph is:\n");
    for (const auto &vertex : vertices)
      output += vertex->getId() + "\n";
    output = output.substr(0, output.size() - 1); // eliminate the last newline character
    return {output};
  });

  console.documentCommand("list_adj", "Display all the vertices adjacent with the given vertex");
  console.registerCommand("list_adj", [&](const auto& args) -> CommandResult {
    if (args.size() != 2)
      throw InvalidUsageError("Usage: list_adj <vertex_id>");

    graph::idT vertexId = args[1];
    std::vector<graph::Edge> edges = graphService.getAdjacentEdges(vertexId);
    if (edges.empty())
      return {"The vertex is isolated!"};
    std::string verticesSeparator = "--";
    if (graphService.getGraphType() == graph::GraphType::Directed) 
      verticesSeparator = "->";
    std::string output = std::format("The {} adjacent to {} {}:\n", 
                                     edges.size() > 1 ? "vertices" : "vertex", 
                                     vertexId,
                                     edges.size() > 1 ? "are" : "is"
                                     );
    for (const auto &[_, toId, cost] : edges)
      output += verticesSeparator + " " + toId + " (" + std::to_string(cost) + ")\n";
    output = output.substr(0, output.size() - 1); // eliminate the last newline character
    return {};
  });
  

  console.documentCommand("list_edges", "Display all the edges in the graph");
  console.registerCommand("list_edges", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: list_edges");
    std::vector<graph::Edge> edges = graphService.getEdges();
    if (edges.empty())
      return {"The graph contains no edges!"};
    std::string verticesSeparator = "--";
    if (graphService.getGraphType() == graph::GraphType::Directed) 
      verticesSeparator = "->";
    std::string output = "The edges are:\n";
    for (const auto &[fromId, toId, cost] : edges)
      output += fromId + " " + verticesSeparator + " " + toId + " (" + std::to_string(cost) + ")\n";
    output = output.substr(0, output.size() - 1); // eliminate the last newline character
    return {output};
  });

  console.documentCommand("load_graph", "Loads a graph from a file");
  console.registerCommand("load_graph", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: load_graph <graph_type> <file_path>");
    std::string graphType = args[1];
    std::string path = args[2];
    graphService.loadGraph(path, graphType);
    return {"Graph loaded successfully"};
  });

  console.documentCommand("save_graph", "Saves the graph to file");
  console.registerCommand("save_graph", [&](const auto& args) -> CommandResult {
    if (args.size() != 2 && args.size() != 1)
      throw InvalidUsageError("Usage: save_graph [file_path]");
    std::string path = "graph.txt";
    if (args.size() == 2)
      path = args[1];
    graphService.saveGraph(path);
    return {"Graph saved successfully"};
  });

  console.documentCommand("get_connected_components", "Returns the connected components of the undirected graph");
  console.registerCommand("get_connected_components", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: get_connected_components");
    auto graphs = graphService.getConnectedComponentsOfUnorderedGraph();
    std::string output = "";
    int connected_component_id = 1;
    for (const auto &graph : graphs) {
      output += "Component " + std::to_string(connected_component_id++) + "\n";
      for (const auto &[vertexId, _] : graph) {
        output += vertexId + " ";
      }
      output += "\n";
    }
    output = output.substr(0, output.length() - 1);
    return {output};
  });

  console.documentCommand("get_lowest_cost_walk", "Returns the lowest cost walk between two vertices");
  console.registerCommand("get_lowest_cost_walk", [&](const auto& args) -> CommandResult {
    if (args.size() != 3)
      throw InvalidUsageError("Usage: get_shortest_path <start> <end>");
    std::string start = args[1];
    std::string end = args[2];
    auto walkResult = graphService.getLowestCostWalk(start, end);
    std::vector<std::string> cheapestPath = walkResult.first;
    int cost = walkResult.second;
    if (cheapestPath.size() == 0)
      return {"There is not path between " + start + " and " + end};
    std::string output = "The shortest path between " + start + " and " + end + " is:\n";
    for (const auto &vertex : cheapestPath) {
      output += vertex + " ";
    }
    output += "\nWith cost " + std::to_string(cost);
    return {output};
  });

  console.documentCommand("get_topological_sort", "Returns the vertices topologically sorted");
  console.registerCommand("get_topological_sort", [&](const auto& args) -> CommandResult {
    if (args.size() != 1)
      throw InvalidUsageError("Usage: get_topological_sort");
    std::string output = "";
    for (const auto &v : graphService.topologicalSort()) {
      output += v + " ";
    }
    if (output == "")
      return {"Unable to topologically sort"};
    return {output};
  });
}
