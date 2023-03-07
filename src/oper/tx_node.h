#ifndef DBTRAIN_TX_NODE_H
#define DBTRAIN_TX_NODE_H

#include "oper_node.h"

namespace dbtrain {

class BeginNode : public OperNode {
 public:
  BeginNode();
  ~BeginNode() = default;
  RecordList Next() override;
  void Display(int depth) const override;
};

class CommitNode : public OperNode {
 public:
  CommitNode();
  ~CommitNode() = default;
  RecordList Next() override;
  void Display(int depth) const override;
};

class AbortNode : public OperNode {
 public:
  AbortNode();
  ~AbortNode() = default;
  RecordList Next() override;
  void Display(int depth) const override;
};

class SignalNode : public OperNode {
 public:
  SignalNode(string signal_name);
  ~SignalNode() = default;
  RecordList Next() override;
  void Display(int depth) const override;

 private:
  string signal_name_;
};

class WaitNode : public OperNode {
 public:
  WaitNode(string signal_name);
  ~WaitNode() = default;
  RecordList Next() override;
  void Display(int depth) const override;

 private:
  string signal_name_;
};

}  // namespace dbtrain

#endif