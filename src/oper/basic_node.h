#ifndef DBTRAIN_MATH_NODE_H
#define DBTRAIN_MATH_NODE_H

#include "conditions/condition.h"
#include "oper_node.h"

namespace dbtrain {

class ProjectNode : public OperNode {
 public:
  ProjectNode(OperNode *child, const vector<int> &idx_vec);
  ~ProjectNode() = default;
  RecordList Next() override;
  double Cost() override;

  virtual void Display(int depth) const override;

 private:
  vector<int> proj_idxs;
};

class FilterNode : public OperNode {
 public:
  FilterNode(OperNode *child, Condition *cond);
  ~FilterNode();
  RecordList Next() override;
  double Cost() override;

  virtual void Display(int depth) const override;

  void SetFilterTable(string tname);

 private:
  Condition *cond_;
  string tname_;
};

}  // namespace dbtrain

#endif
