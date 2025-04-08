#pragma once
#include "graph.hpp"

namespace graph {
  template <typename T>
  class UndirectedGraph : public Graph<T>{
  private:
    std::unordered_map<T, std::unordered_set<T>> adjacency;	
  std::unordered_set<T> vertices;
  public:
      UndirectedGraph() {}
      bool isVertex(const T& v) const override;
      bool isEdge(const T& from, const T& to) const override;
      void addVertex(const T& v) override;
      void removeVertex(const T& v) override;
      void addEdge(const T& from, const T& to) override;
      void removeEdge(const T& from, const T& to) override;

      std::unordered_set<T>::const_iterator begin() const override;
      std::unordered_set<T>::const_iterator end() const override;
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
    std::unordered_set<T>::const_iterator UndirectedGraph<T>::begin() const {
      return vertices.begin();
    }


    template <typename T>
    std::unordered_set<T>::const_iterator UndirectedGraph<T>::end() const {
      return vertices.end();
    }
}
