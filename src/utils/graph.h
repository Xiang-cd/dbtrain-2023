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
  void AddVertex(const T &v) { data_.push_back(v); }
  void AddEdge(const T &u, const T &v) {
    if (adjace_map_.find(u) == adjace_map_.end())
      adjace_map_[u] = {v};
    else
      adjace_map_[u].push_back(v);

    if (adjace_map_.find(v) == adjace_map_.end())
      adjace_map_[v] = {u};
    else
      adjace_map_[v].push_back(u);
  }
  vector<T> Adjace(const T &v) {
    if (adjace_map_.find(v) == adjace_map_.end()) return {};
    return adjace_map_[v];
  }

 private:
  vector<T> data_;
  std::map<T, std::vector<T>> adjace_map_;
};

}  // namespace dbtrain

#endif
