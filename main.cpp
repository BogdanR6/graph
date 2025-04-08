#include <iostream>
#include <assert.h>
#include "interface/graph.hpp"


int main() {
	graph::Graph<int> g;
	g.addVertex(1);
	g.addVertex(2);
	g.addVertex(3);
	g.removeVertex(2);
	try {
		g.addVertex(1);
		assert(false);
	} catch (std::runtime_error& e) {
		std::cout << e.what() << '\n';
	}
	try {
		g.removeVertex(2);
		assert(false);
	} catch (std::runtime_error& e) {
		std::cout << e.what() << '\n';
	}
	return 0;
}
