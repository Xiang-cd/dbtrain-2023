#include "histogram.h"

#include <algorithm>

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
  // LAB 5 END
}

double Histogram::LowerBound(double lower) const {
  // TODO: 统计并返回大于等于lower的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  // LAB 5 END
}

double Histogram::UpperBound(double upper) const {
  // TODO: 统计并返回小于upper的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  // LAB 5 END
}

double Histogram::RangeBound(double lower, double upper) const {
  // TODO: 统计并返回介于lower和upper之间的数据比例
  // TIPS: 注意超界判断
  // TIPS: 注意返回比例，不是总量
  // LAB 5 BEGIN
  // LAB 5 END
}

}  // namespace dbtrain
