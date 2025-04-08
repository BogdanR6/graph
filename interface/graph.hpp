#pragma once
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace graph {
  template<typename T>
  using VertexIterator = typename std::unordered_set<T>::const_iterator;

  template<typename T>
  class Graph {
  public:
    virtual ~Graph() = default;


    virtual void addVertex(const T& v) = 0;
    virtual void addEdge(const T& from, const T& to) = 0;
		virtual void removeVertex(const T& v) = 0;
		virtual void removeEdge(const T& from, const T& to) = 0;
    virtual bool isVertex(const T& v) const = 0;
    virtual bool isEdge(const T& from, const T& to) const = 0;
    
    virtual VertexIterator<T> begin() const = 0;
    virtual VertexIterator<T> end() const = 0;
  };
	
	template <typename A, typename B>
	using AdjacencyMap = std::unordered_map<A, std::unordered_set<B>>;

	template <typename T>
	class UndirectedGraph : public Graph<T>{
	private:
		AdjacencyMap<T, T> adjacency;	
    std::unordered_set<T> vertices;
	public:
		UndirectedGraph() {}
		bool isVertex(const T& v) const override;
		bool isEdge(const T& from, const T& to) const override;
		void addVertex(const T& v) override;
		void removeVertex(const T& v) override;
		void addEdge(const T& from, const T& to) override;
		void removeEdge(const T& from, const T& to) override;

    VertexIterator<T> begin() const override;
    VertexIterator<T> end() const override;
	};


	template <typename T>
	bool UndirectedGraph<T>::isVertex(const T& v) const {
		return adjacency.find(v) != adjacency.end();
	}


	template <typename T>
	bool UndirectedGraph<T>::isEdge(const T& from, const T& to) const {
		return isVertex(from) &&
				 	 isVertex(to) &&
					 adjacency.at(from).find(to) != adjacency.at(from).end();
	}

	
	template <typename T>
	void UndirectedGraph<T>::addVertex(const T& v) {
		if (isVertex(v))
			throw std::runtime_error("Vertex Already added");

		adjacency[v] = std::unordered_set<T>();
    vertices.insert(v);
	}


	template <typename T>
	void UndirectedGraph<T>::removeVertex(const T& v) {
		if (!isVertex(v))
			throw std::runtime_error("Vertex not in the graph");

		for (const auto& ver : adjacency[v]) {
			adjacency[ver].erase(v);								
		}
		adjacency.erase(v);
    vertices.erase(v);
	}


	template <typename T>
	void UndirectedGraph<T>::addEdge(const T& from, const T& to) {
		if (!isVertex(from))
			throw std::runtime_error("from is not in the graph");
		if (!isVertex(to))
			throw std::runtime_error("to is not in the graph");
		if (isEdge(from, to))
			throw std::runtime_error("The edge already exists");
		
		adjacency[from].insert(to);
		adjacency[to].insert(from);
	}
	

	template <typename T>
	void UndirectedGraph<T>::removeEdge(const T& from, const T& to) {
		if (!isVertex(from))
			throw std::runtime_error("from is not in the graph");
		if (!isVertex(to))
			throw std::runtime_error("to is not in the graph");
		if (!isEdge(from, to))
			throw std::runtime_error("The edge does not exist");
		
		adjacency[from].erase(to);
		adjacency[to].erase(from);
	}

	template <typename T>
  VertexIterator<T> UndirectedGraph<T>::begin() const {
    return vertices.begin();
  }


	template <typename T>
  VertexIterator<T> UndirectedGraph<T>::end() const {
    return vertices.end();
  }
	

	// template <typename T>
	// struct Iterator {
	// 	using iterator_category = std::forward_iterator_tag;
  //   using difference_type   = std::ptrdiff_t;
  //   using value_type        = T;
  //   using pointer           = T*;  
  //   using reference         = T&;  

	// 	Iterator(pointer ptr) : m_ptr(ptr) {}
	// 	reference operator*() const { return *m_ptr; }
  //   pointer operator->() { return m_ptr; }

  //   // Prefix increment
  //   Iterator& operator++() { m_ptr++; return *this; }  

  //   // Postfix increment
  //   Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

  //   friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
  //   friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }; 
	// private:
	// 	pointer m_ptr;
	// };
  
  template <typename A, typename B>
  using CostMap = std::unordered_map<std::pair<A, B>, int>;

  template <typename T>
  class DirectedGraph : public Graph<T> {
  private:
    AdjacencyMap<T, T> inAdjacency;
    AdjacencyMap<T, T> outAdjacency;
    CostMap<T, T> costs; 
    std::unordered_set<T> vertices;
  public:
		bool isVertex(const T& v) const override;
		bool isEdge(const T& from, const T& to) const override;
		void addVertex(const T& v) override;
		void removeVertex(const T& v) override;
		void addEdge(const T& from, const T& to) override;
		void removeEdge(const T& from, const T& to) override;
    VertexIterator<T> begin() const override; 
    VertexIterator<T> end() const override; 

    int getInDegree(const T& v);
    int getOutDegree(const T& v);
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
		if (isEdge(from, to))
			throw std::runtime_error("The edge already exists");
		
		outAdjacency[from].insert(to);
		inAdjacency[to].insert(from);
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
	}


  template <typename T>
  VertexIterator<T> DirectedGraph<T>::begin() const {
    return vertices.begin();
  }


  template <typename T>
  VertexIterator<T> DirectedGraph<T>::end() const {
    return vertices.end();
  }

}
