#pragma once
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace graph {

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
    
    virtual std::unordered_set<T>::const_iterator begin() const = 0;
    virtual std::unordered_set<T>::const_iterator end() const = 0;
  };
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

  template <typename T>
  class InboundEdgesIterator;

  template <typename T>
  class OutboundEdgesIterator;
  
  

  // template <typename T>
  // OutboundEdgesIterator<T> DirectedGraph<T>::getOutboundEdges(const T& v) {
  //   if (!isVertex(v))
  //     throw std::runtime_error("Vertex not in the graph");
  //   return OutboundEdgesIterator<T>(*this, v);
  // }


  // template <typename T>
  // InboundEdgesIterator<T> DirectedGraph<T>::getInboundEdges(const T& v) {
  //   if (!isVertex(v))
  //     throw std::runtime_error("Vertex not in the graph");
  //   return InboundEdgesIterator<T>(*this, v);
  // }
  
  // template <typename T>
  // class InboundEdgesIterator {
  // private:
  //   const DirectedGraph<T>& graph;
  //   const T& vertex;
  // public:
  //   InboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
  //           : graph(graph), vertex(vertex) {}
  //   VertexIterator<T> begin() const {
  //     return graph.inAdjacency[vertex].begin();
  //   }
  //   VertexIterator<T> end() const {
  //     return graph.inAdjacency[vertex].end();
  //   }
  // };


  // template <typename T>
  // class OutboundEdgesIterator {
  // private:
  //   const DirectedGraph<T>& graph;
  //   const T& vertex;
  // public:
  //   OutboundEdgesIterator(const DirectedGraph<T>& graph, const T& vertex)
  //           : graph(graph), vertex(vertex) {}
  //   VertexIterator<T> begin() const {
  //     return graph.outAdjacency[vertex].begin();
  //   }
  //   VertexIterator<T> end() const {
  //     return graph.outAdjacency[vertex].end();
  //   }
  // };
}
