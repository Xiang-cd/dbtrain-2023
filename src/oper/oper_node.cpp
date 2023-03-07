#include "oper_node.h"

namespace dbtrain {

OperNode::OperNode(const vector<OperNode *> &childs) : childs_(childs) {}

OperNode::~OperNode() {
  for (const auto &child : childs_) delete child;
}

double OperNode::Cost() {
  if (childs_.size() == 1) return childs_[0]->Cost();
  return 0;
}

void OperNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Node:\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

}  // namespace dbtrain
