#include "histogram.h"

#include <algorithm>
#include "debug-print.hpp"
namespace dbtrain {

Histogram::Histogram(int num_buckets) : num_buckets_(num_buckets) {
  total_ = 0;
  counts_ = new int[num_buckets];
  for (int i = 0; i < num_buckets; i++) {
    counts_[i] = 0;
  }
}

Histogram::~Histogram() { delete[] counts_; }

int Histogram::GetRecordNum() const { return total_; }

void Histogram::Init(const vector<double> &val_list) {
  // TODO: 根据val_list生成直方图
  // TIPS: 先判断数据上下界范围，之后按照同数量划分区间
  // TIPS: 按照划分区间统计各个桶内的数据量
  // TIPS: 同时记录总数据量
  // LAB 5 BEGIN
//  LAB5Print("Histogram::Init>> total size:", val_list.size());
  max_value_ = * std::max_element(val_list.begin(), val_list.end());
  min_value_ = * std::min_element(val_list.begin(), val_list.end());
  width_ = (max_value_ - min_value_) / num_buckets_;
  total_ = val_list.size();
  for (auto d:val_list){
    int index = (int)((d - min_value_) / width_);
    index = std::min(index, num_buckets_ - 1);
    counts_[index] ++;
  }

  // LAB 5 END
}

double Histogram::LowerBound(double lower) const {
  // TODO: 统计并返回大于等于lower的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  // 落在第几个桶
  int index = (int)((lower - min_value_) / width_);
  if (index < 0) return 1.;
  if (index >= num_buckets_) return 0.;

  int greater_num = 0;
  for (int i = index; i < num_buckets_; ++i) {
    greater_num += counts_[i];
  }
  return double (greater_num) / double (total_);
  // LAB 5 END
}

double Histogram::UpperBound(double upper) const {
  // TODO: 统计并返回小于upper的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  // 落在第几个桶
  int index = (int)((upper - min_value_) / width_);
  if (index >= num_buckets_) return 1.;
  if (index < 0) return 0.;

  int smaller_num = 0;
  for (int i = 0; i < index; ++i){
    smaller_num += counts_[i];
  }
  return double (smaller_num) / double(total_);
  // LAB 5 END
}

double Histogram::RangeBound(double lower, double upper) const {
  // TODO: 统计并返回介于lower和upper之间的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  int index_low = (int)((lower - min_value_) / width_);
  int index_high = (int)((upper - min_value_) / width_);
  index_low = std::max(index_low, 0);
  index_high = std::min(index_high, num_buckets_);
  int mid_num = 0;
  for (int i = index_low; i < index_high; ++i) {
    mid_num += counts_[i];
  }
  return double (mid_num) / double (total_);
  // LAB 5 END
}

}  // namespace dbtrain
