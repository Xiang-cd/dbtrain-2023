#include "join_node.h"

#include "optim/stats_manager.h"
#include "utils/debug-print.hpp"
namespace dbtrain {

JoinNode::JoinNode(OperNode *left, OperNode *right, JoinCondition *cond) : OperNode({left, right}), cond_(cond) {}

JoinNode::~JoinNode() { delete cond_; }

OperNode *JoinNode::GetLeft() const { return childs_[0]; }

OperNode *JoinNode::GetRight() const { return childs_[1]; }

RecordList JoinNode::Next() {
  // TODO: 连接运算
  // TIPS: 通过 GetLeft()->Next() 和 GetRight()->Next() 获取记录
  // TIPS: 通过 cond_->Fit() 判断是否符合条件
  // LAB 4 BEGIN
  LAB4Print("JoinNode: lidx", cond_->idx_left_, " ridx:", cond_->idx_right_);
  auto ll = GetLeft()->Next();
  auto rl =  GetRight()->Next();
  RecordList lla, rla;
  while (!ll.empty()){
    lla.insert(lla.end(), ll.begin(), ll.end());
    ll = GetLeft()->Next();
  }

  while (!rl.empty()){
    rla.insert(rla.end(), rl.begin(), rl.end());
    rl = GetRight()->Next();
  }


  RecordList outlist = {};
  for (int i = 0; i < lla.size(); ++i) {
    for (int j = 0; j < rla.size(); ++j) {
      if (cond_->Fit(lla[i], rla[j]))
        outlist.push_back(Concat(lla[i], rla[j]));
    }
  }
  return outlist;
  // LAB 4 END
}

double JoinNode::Cost() {
  // 估计Join后的基数
  // TODO: （高级功能）Join的基数估计
  // TIPS: 注意，基础功能不需要考虑
  // LAB 5 BEGIN
  // LAB 5 END
}

Record *JoinNode::Concat(Record *left, Record *right) const {
  auto rs = left->GetNumField() + right->GetNumField();
  Record *record = new Record();
  for (auto i = 0; i < left->GetNumField(); ++i) record->PushBack(left->GetField(i)->Copy());
  for (auto i = 0; i < right->GetNumField(); ++i) record->PushBack(right->GetField(i)->Copy());
  return record;
}

void JoinNode::SetJoinTable(string ltname, string rtname) {
  ltname_ = ltname;
  rtname_ = rtname;
}

void JoinNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Join Node:");
  // cond_->Display();
  printf("\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

}  // namespace dbtrain
