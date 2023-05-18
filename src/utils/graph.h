#ifndef DBTRAIN_GRAPH_H
#define DBTRAIN_GRAPH_H

#include <map>
#include <set>

#include "defines.h"

namespace dbtrain {

template <class T>
class UndirectedGraph {
 public:
  UndirectedGraph() = default;
  ~UndirectedGraph() = default;
  void AddVertex(const T &v) { data_.insert(v); }
  void AddEdge(const T &u, const T &v) {
    if (adjace_map_.find(u) == adjace_map_.end())
      adjace_map_[u] = {v};
    else
      adjace_map_[u].insert(v);

    if (adjace_map_.find(v) == adjace_map_.end())
      adjace_map_[v] = {u};
    else
      adjace_map_[v].insert(u);
  }
  std::set<T> Adjace(const T &v) {
    if (adjace_map_.find(v) == adjace_map_.end()) return {};
    return adjace_map_[v];
  }
  size_t NodeNum(){
    return data_.size();
  }

 private:
  std::set<T> data_;
  std::map<T, std::set<T>> adjace_map_;
};

}  // namespace dbtrain

#endif
