#ifndef DBTRAIN_JOIN_NODE_H
#define DBTRAIN_JOIN_NODE_H

#include "conditions/join_condition.h"
#include "oper_node.h"

namespace dbtrain {

class JoinNode : public OperNode {
 public:
  JoinNode(OperNode *left, OperNode *right, JoinCondition *cond);
  ~JoinNode();
  RecordList Next() override;
  double Cost() override;

  virtual void Display(int depth) const override;

  void SetJoinTable(string ltname, string rtname);

 protected:
  OperNode *GetLeft() const;
  OperNode *GetRight() const;
  JoinCondition *cond_;
  virtual Record *Concat(Record *left, Record *right) const;

  string ltname_, rtname_;
};

}  // namespace dbtrain

#endif
