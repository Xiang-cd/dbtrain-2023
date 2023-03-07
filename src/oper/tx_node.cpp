#include "tx_node.h"

#include "log/log_manager.h"
#include "tx/signal_manager.h"
#include "tx/tx_manager.h"

namespace dbtrain {

BeginNode::BeginNode() : OperNode({}) {}
RecordList BeginNode::Next() {
  TID tid = std::this_thread::get_id();
  // 事务开始，获取新的事务ID
  XID xid = TxManager::GetInstance().Push(tid);
  // 记录事务开始日志
  LogManager::GetInstance().Begin(xid);
  return {};
}
void BeginNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Begin\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

CommitNode::CommitNode() : OperNode({}) {}
RecordList CommitNode::Next() {
  // 事务提交
  // NoForce策略，不会立即写回
  TID tid = std::this_thread::get_id();
  XID xid = TxManager::GetInstance().Get(tid);
  // 记录事务提交日志，WAL
  LogManager::GetInstance().Commit(xid);
  // 清除线程记录
  TxManager::GetInstance().Pop(tid);
  return {};
}
void CommitNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Commit\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

AbortNode::AbortNode() : OperNode({}) {}
RecordList AbortNode::Next() {
  // 事务中止
  // 调用LogManager::Undo
  TID tid = std::this_thread::get_id();
  XID xid = TxManager::GetInstance().Get(tid);
  // 记录事务中止日志，WAL
  LogManager::GetInstance().Abort(xid);
  // 清除线程记录
  TxManager::GetInstance().Pop(tid);
  return {};
}
void AbortNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Abort\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

SignalNode::SignalNode(string sname) : OperNode({}), signal_name_(sname) {}
RecordList SignalNode::Next() {
  SignalManager &smanager = SignalManager::GetInstance();
  smanager.CreateSignal(signal_name_);
  return {};
}
void SignalNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Signal %s\n", signal_name_.c_str());
  for (const auto &child : childs_) child->Display(depth + 1);
}

WaitNode::WaitNode(string sname) : OperNode({}), signal_name_(sname) {}
RecordList WaitNode::Next() {
  SignalManager &smanager = SignalManager::GetInstance();
  smanager.WaitSignal(signal_name_);
  return {};
}
void WaitNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Wait %s\n", signal_name_.c_str());
  for (const auto &child : childs_) child->Display(depth + 1);
}

}  // namespace dbtrain
