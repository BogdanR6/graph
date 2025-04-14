#pragma once
#include <unordered_set>

namespace graph {

enum class GraphType {
  Directed,
  Undirected
};

template <typename T>
struct Edge {
  T from;
  T to;
  bool operator==(const Edge& other) const {
    return from == other.from && to == other.to;
  }
};

template <typename T>
struct EdgeHash {
  std::size_t operator()(const Edge<T>& e) const {
    return std::hash<T>()(e.from) ^ (std::hash<T>()(e.to) << 1);
  }
};

template <typename T> class Graph {
public:
  virtual ~Graph() = default;
  virtual GraphType getGraphType() const = 0;

  virtual void addVertex(const T &v) = 0;
  virtual void addEdge(const T &from, const T &to, const int& weight = 1) = 0;
  virtual void removeVertex(const T &v) = 0;
  virtual void removeEdge(const T &from, const T &to) = 0;
  virtual bool isVertex(const T &v) const = 0;
  virtual bool isEdge(const T &from, const T &to) const = 0;
  virtual int getNrOfVertices() const = 0;
  virtual int getNrOfEdges() const = 0;
  virtual int getEdgeWeight(const T &from, const T &to) const = 0;
  virtual void clear() = 0;
  // include a get adjacent vertices function

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

//   friend bool operator== (const Iterator& a, const Iterator& b) { return
//   a.m_ptr == b.m_ptr; }; friend bool operator!= (const Iterator& a, const
//   Iterator& b) { return a.m_ptr != b.m_ptr; };
// private:
// 	pointer m_ptr;
// };
} // namespace graph
