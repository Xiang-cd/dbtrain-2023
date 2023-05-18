#include "basic_node.h"

#include <cmath>

#include "conditions/conditions.h"
#include "optim/stats_manager.h"
#include "record/fields.h"
#include "utils/debug-print.hpp"


namespace dbtrain {

typedef std::pair<double, double> RangePair;

ProjectNode::ProjectNode(OperNode *child, const vector<int> &idx_vec) : OperNode({child}) { proj_idxs = idx_vec; }

RecordList ProjectNode::Next() {
  OperNode *child = childs_[0];
  RecordList inlist = child->Next();
  RecordList outlist{};
  // 投影运算
  for (const auto &record : inlist) {
    Record *proj_record = new Record();
    for (const auto &idx : proj_idxs) proj_record->PushBack(record->GetField(idx)->Copy());
    outlist.push_back(proj_record);
  }
  // 删除无用指针
  for (const auto record : inlist) {
    delete record;
  }
  return outlist;
}

double ProjectNode::Cost() {
  OperNode *child = childs_[0];
  return child->Cost();
}

void ProjectNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Project Node:");
  // for (const auto& idx : proj_idxs) printf("%d ", idx);
  printf("\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

FilterNode::FilterNode(OperNode *child, Condition *cond) : OperNode({child}), cond_(cond) {
  if (dynamic_cast<AndCondition *>(cond) != nullptr)LAB5Print("filter node init with and oncd");

}

FilterNode::~FilterNode() { delete cond_; }

RecordList FilterNode::Next() {
  OperNode *child = childs_[0];
  RecordList inlist{};
  RecordList outlist{};
  while (outlist.empty()) {
    inlist = child->Next();
    if (inlist.empty()) break;
    for (const auto &record : inlist) {
      if (cond_->Fit(record))
        outlist.push_back(record);
      else
        delete record;
    }
  }
  return outlist;
}

double GetFieldValue(Field *field) {
  if (field->GetType() == FieldType::INT) {
    return dynamic_cast<IntField *>(field)->GetValue();
  } else if (field->GetType() == FieldType::FLOAT) {
    return dynamic_cast<FloatField *>(field)->GetValue();
  } else {
    return 0;
  }
}

int UpdateBound(Condition *cond, double &lower, double &upper) {
  // 自动判断一个AlgebraCondition并更新上下界，仅限于数值型
  // 返回约束的列号
  EqualCondition *equal_cond = dynamic_cast<EqualCondition *>(cond);
  if (equal_cond != nullptr) {
    double bound = GetFieldValue(equal_cond->GetField());
    if (bound > lower) lower = bound;
    if (equal_cond->GetField()->GetType() == FieldType::INT) bound = std::ceil(bound + EPSILON);
    if (bound + EPSILON < upper) upper = bound + EPSILON;
    return equal_cond->GetIdx();
  }
  GreaterCondition *great_cond = dynamic_cast<GreaterCondition *>(cond);
  if (great_cond != nullptr) {
    double bound = GetFieldValue(great_cond->GetField());
    if (bound > lower) lower = bound;
    return great_cond->GetIdx();
  }
  LessCondition *less_cond = dynamic_cast<LessCondition *>(cond);
  if (less_cond != nullptr) {
    double bound = GetFieldValue(less_cond->GetField());
    if (bound + EPSILON < upper) upper = bound + EPSILON;
    return less_cond->GetIdx();
  }
  assert(false);
}

double FilterNode::Cost() {
  // TIPS: 基础功能不需要考虑Or条件
  OrCondition *or_cond = dynamic_cast<OrCondition *>(cond_);
  if (or_cond != nullptr) return childs_[0]->Cost();
  // TIPS: 本次实验中仅存在3种情况:(1)无条件;(2)单过滤条件;(3)And的多过滤条件。均仅需要考虑数值型
  // TIPS: 注意处理AndCondition的方式需要和LAB 3合成AndCondition的方式匹配
  // TIPS: 首先，完成无过滤条件情景的Cost估计，直接返回子节点Cost即可
  // TIPS: 之后，考虑单个过滤条件。可以利用UpdateBound更新过滤范围
  // TIPS: 最后，考虑And多条件过滤情景，需要考虑相同列的过滤条件合并
  // TIPS: 此外，多个属性同时存在过滤条件时简化假设各个属性互相独立
  // TIPS: 利用StatsManager的相关函数可以估计过滤条件所占比例
  // LAB 5 BEGIN

  if (cond_ == nullptr) return childs_[0]->Cost();


  auto * single_cond = dynamic_cast<AlgebraCondition *>(cond_);
  if (single_cond != nullptr){
    double  lower = -1e10, upper = 1e10;
    auto idx = UpdateBound(cond_, lower, upper);
    auto & stmg = StatsManager::GetInstance();
    GreaterCondition *great_cond = dynamic_cast<GreaterCondition *>(cond_);
    if (great_cond != nullptr) {
      return stmg.LowerBound(tname_, idx, lower) * childs_[0]->Cost();
    }
    LessCondition *less_cond = dynamic_cast<LessCondition *>(cond_);
    if (less_cond != nullptr) {
      return stmg.UpperBound(tname_, idx, upper) * childs_[0]->Cost();
    }
    return stmg.RangeBound(tname_, idx, lower, upper) * childs_[0]->Cost();
  }


  // LAB 5 END
}

void FilterNode::SetFilterTable(string tname) { tname_ = tname; }

void FilterNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Filter Node:");
  // cond_->Display();
  printf("\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

}  // namespace dbtrain
