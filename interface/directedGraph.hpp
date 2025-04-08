#pragma once
#include "graph.hpp"
#include <stdexcept>

namespace std { // for the map with the map of keys
    template <typename T>
    struct hash<std::pair<T, T>> {
        size_t operator()(const std::pair<T, T>& p) const {
            // Combine the hashes of the two elements in the pair
            size_t h1 = hash<T>{}(p.first);
            size_t h2 = hash<T>{}(p.second);
            return h1 ^ (h2 << 1);  // Combine the two hashes
        }
    };
}

namespace graph {

  template <typename T>
  class InboundEdgesIterator;

  template <typename T>
  class OutboundEdgesIterator;


  template <typename T>
  class DirectedGraph : public Graph<T> {
  private:
    bool weighted = false;
    bool determined_weighted = false;
    std::unordered_map<T, std::unordered_set<T>> inAdjacency;
    std::unordered_map<T, std::unordered_set<T>> outAdjacency;
    std::unordered_map<std::pair<T, T>, int> weights; 
    std::unordered_set<T> vertices;
    friend class InboundEdgesIterator<T>;
    friend class OutboundEdgesIterator<T>;
  public:
    DirectedGraph() : inAdjacency(), outAdjacency(), weights(), vertices() {}
		bool isVertex(const T& v) const override;
		bool isEdge(const T& from, const T& to) const override;
		void addVertex(const T& v) override;
		void removeVertex(const T& v) override;
		void addEdge(const T& from, const T& to) override;
		void removeEdge(const T& from, const T& to) override;
    std::unordered_set<T>::const_iterator begin() const override; 
    std::unordered_set<T>::const_iterator end() const override; 

    void addWEdge(const T& from, const T& to, const int& weight);
    int getInDegree(const T& v);
    int getOutDegree(const T& v);

    
    OutboundEdgesIterator<T> getOutboundEdges(const T& v);
    InboundEdgesIterator<T> getInboundEdges(const T& v);
  };

	template <typename T>
	bool DirectedGraph<T>::isVertex(const T& v) const {
		return vertices.find(v) != vertices.end();
	}


	template <typename T>
	bool DirectedGraph<T>::isEdge(const T& from, const T& to) const {
		return isVertex(from) &&
				 	 isVertex(to) &&
					 outAdjacency.at(from).find(to) != outAdjacency.at(from).end();
	}

	
	template <typename T>
	void DirectedGraph<T>::addVertex(const T& v) {
		if (isVertex(v))
			throw std::runtime_error("Vertex Already added");

		outAdjacency[v] = std::unordered_set<T>();
		inAdjacency[v] = std::unordered_set<T>();
    vertices.insert(v);
	}


	template <typename T>
	void DirectedGraph<T>::removeVertex(const T& v) {
		if (!isVertex(v))
			throw std::runtime_error("Vertex not in the graph");

		for (const auto& ver : outAdjacency[v]) {
			inAdjacency[ver].erase(v);								
		}
		outAdjacency.erase(v);
    vertices.erase(v);
	}


	template <typename T>
	void DirectedGraph<T>::addEdge(const T& from, const T& to) {
		if (!isVertex(from))
			throw std::runtime_error("from is not in the graph");
		if (!isVertex(to))
			throw std::runtime_error("to is not in the graph");
    if (determined_weighted && weighted)
			throw std::runtime_error("The graph is weighted. Can't add unweighted edges to it");
		if (isEdge(from, to))
			throw std::runtime_error("The edge already exists");
		
		outAdjacency[from].insert(to);
		inAdjacency[to].insert(from);
    determined_weighted = true;
    weighted = false;
	}
	

	template <typename T>
	void DirectedGraph<T>::removeEdge(const T& from, const T& to) {
		if (!isVertex(from))
			throw std::runtime_error("from is not in the graph");
		if (!isVertex(to))
			throw std::runtime_error("to is not in the graph");
		if (!isEdge(from, to))
			throw std::runtime_error("The edge does not exist");
		
		outAdjacency[from].erase(to);
		inAdjacency[to].erase(from);
    if (weighted)
      weights.erase(std::make_pair(from, to));
	}


  template <typename T>
  void DirectedGraph<T>::addWEdge(const T& from, const T& to, const int& weight) {
		if (!isVertex(from))
			throw std::runtime_error("from is not in the graph");
		if (!isVertex(to))
			throw std::runtime_error("to is not in the graph");
    if (determined_weighted && !weighted)
			throw std::runtime_error("The graph is unweighted. Can't add weighted edges to it");
		if (isEdge(from, to))
			throw std::runtime_error("The edge already exists");
    
		outAdjacency[from].insert(to);
		inAdjacency[to].insert(from);
    weights[std::make_pair(from, to)] = weight;
    determined_weighted = true;
    weighted = true;
  }


  template <typename T>
  std::unordered_set<T>::const_iterator DirectedGraph<T>::begin() const {
    return vertices.begin();
  }


  template <typename T>
  std::unordered_set<T>::const_iterator DirectedGraph<T>::end() const {
    return vertices.end();
  }

  template <typename T>
  int DirectedGraph<T>::getInDegree(const T& v) {
    if (!isVertex(v))
      throw std::runtime_error("Vertex is not in the graph");
    return inAdjacency.at(v).size();
  } 


  template <typename T>
  int DirectedGraph<T>::getOutDegree(const T& v) {
    if (!isVertex(v))
      throw std::runtime_error("Vertex is not in the graph");
    return outAdjacency.at(v).size();
  }


  template <typename T>
  OutboundEdgesIterator<T> DirectedGraph<T>::getOutboundEdges(const T& v) {
    if (!isVertex(v))
      throw std::runtime_error("Vertex not in the graph");
    return OutboundEdgesIterator<T>(*this, v);
  }


  template <typename T>
  InboundEdgesIterator<T> DirectedGraph<T>::getInboundEdges(const T& v) {
    if (!isVertex(v))
      throw std::runtime_error("Vertex not in the graph");
    return InboundEdgesIterator<T>(*this, v);
  }
  
  template <typename T>
  class InboundEdgesIterator {
  private:
    const DirectedGraph<T>& graph;
    const T& vertex;
  public:
    InboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
            : graph(graph), vertex(vertex) {}
    std::unordered_set<T>::const_iterator begin() const {
      return graph.inAdjacency.at(vertex).begin();
    }
    std::unordered_set<T>::const_iterator end() const {
      return graph.inAdjacency.at(vertex).end();
    }
  };


  template <typename T>
  class OutboundEdgesIterator {
  private:
    const DirectedGraph<T>& graph;
    const T& vertex;
  public:
    OutboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
            : graph(graph), vertex(vertex) {}

    std::unordered_set<T>::const_iterator begin() const {
      return graph.outAdjacency.at(vertex).begin();
    }

    std::unordered_set<T>::const_iterator end() const {
      return graph.outAdjacency.at(vertex).end();
    }
  };
}
