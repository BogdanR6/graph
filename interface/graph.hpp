#pragma once
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace graph {
	
	template <typename A, typename B>
	using AdjacencyMap = std::unordered_map<A, std::unordered_set<B>>;

	template <class T>
	class Graph {
	private:
		AdjacencyMap<T, T> adjacency;	
	public:
		Graph() {}
		bool isVertex(T v);
		bool isEdge(T v1, T v2);
		void addVertex(T v);
		void removeVertex(T v);
		void addEdge(T v1, T v2);
		void removeEdge(T v1, T v2);

		bool operator== (const Graph& other_graph); 
		struct Iterator;

		Iterator begin();
		Iterator end();
	};


	template <typename T>
	bool Graph<T>::isVertex(T v) {
		return adjacency.find(v) != adjacency.end();
	}


	template <typename T>
	bool Graph<T>::isEdge(T v1, T v2) {
		return isVertex(v1) &&
				 	 isVertex(v2) &&
					 adjacency[v1].find(v2) != adjacency[v1].end();
	}

	
	template <typename T>
	void Graph<T>::addVertex(T v) {
		if (isVertex(v))
			throw std::runtime_error("Vertex Already added");

		adjacency[v] = std::unordered_set<T>();
	}


	template <typename T>
	void Graph<T>::removeVertex(T v) {
		if (!isVertex(v))
			throw std::runtime_error("Vertex not in the graph");

		for (const auto& ver : adjacency[v]) {
			adjacency[ver].erase(v);								
		}
		adjacency.erase(v);
	}


	template <typename T>
	void Graph<T>::addEdge(T v1, T v2) {
		if (!isVertex(v1))
			throw std::runtime_error("v1 is not in the graph");
		if (!isVertex(v2))
			throw std::runtime_error("v2 is not in the graph");
		if (isEdge(v1, v2))
			throw std::runtime_error("The edge already exists");
		
		adjacency[v1].insert(v2);
		adjacency[v2].insert(v1);
	}
	

	template <typename T>
	void Graph<T>::removeEdge(T v1, T v2) {
		if (!isVertex(v1))
			throw std::runtime_error("v1 is not in the graph");
		if (!isVertex(v2))
			throw std::runtime_error("v2 is not in the graph");
		if (!isEdge(v1, v2))
			throw std::runtime_error("The edge does not exist");
		
		adjacency[v1].erase(v2);
		adjacency[v2].erase(v1);
	}
	

	template <typename T>
	struct Iterator {
		using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;  
    using reference         = T&;  

		Iterator(pointer ptr) : m_ptr(ptr) {}
		reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    // Prefix increment
    Iterator& operator++() { m_ptr++; return *this; }  

    // Postfix increment
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }; 
	private:
		pointer m_ptr;
	};
}

