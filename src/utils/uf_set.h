#ifndef DBTRAIN_UF_SET_H
#define DBTRAIN_UF_SET_H

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace dbtrain {

template <class T>
class UFSet {
 public:
  UFSet(const std::vector<T> &data) : data_(data) {
    set_id_ = std::vector<int>(data.size(), 0);
    for (auto i = 0; i < data.size(); ++i) {
      data_map_[data_[i]] = i;
      set_id_[i] = i;
    }
  }
  void Union(const T &x, const T &y) {
    int xi = Find_(Get_(x)), yi = Find_(Get_(y));
    set_id_[xi] = yi;
  }
  T Find(const T &x) {
    return data_[Find_(Get_(x))];
  }
  std::vector<T> FindAll(const T &x) {
    int xi = Find_(Get_(x));
    std::vector<T> val_vec{};
    for (int i = 0; i < data_.size(); ++i) {
      if (Find_(i) == xi) val_vec.push_back(data_[i]);
    }
    return val_vec;
  }

 private:
  std::vector<T> data_;
  std::vector<int> set_id_;
  std::unordered_map<T, int> data_map_;

  int Get_(const T &x) { return data_map_[x]; }
  int Find_(int xi) {
    if (xi != set_id_[xi]) set_id_[xi] = Find_(set_id_[xi]);
    return set_id_[xi];
  }
};

}  // namespace dbtrain

#endif
