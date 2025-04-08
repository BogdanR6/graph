#include <iostream>
#include <assert.h>
// #include "interface/undirectedGraph.hpp"
#include "interface/directedGraph.hpp"


int main() {
	graph::DirectedGraph<int> g;
	g.addVertex(1);
	g.addVertex(2);
	g.addVertex(3);
	g.addVertex(4);
	g.addVertex(5);
	g.addVertex(6);
	g.addVertex(7);

	g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(1, 4);

	g.addEdge(5, 2);
	g.addEdge(6, 2);

	for (const auto& v : g) {
		std::cout << v << ' ';
	}
	std::cout << '\n';
	std::cout << g.getInDegree(2) << '\n';
	std::cout << g.getOutDegree(1) << '\n';
	for (const auto& v : g.getOutboundEdges(1)) {
		std::cout << v << ' ';
	}
	std::cout << '\n';
	for (const auto& v : g.getInboundEdges(2)) {
		std::cout << v << ' ';
	}
	std::cout << '\n';
	return 0;
}
