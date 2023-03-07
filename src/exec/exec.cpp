#include "exec.h"

#include "log/log_manager.h"
#include "tx/tx_manager.h"

namespace dbtrain {

Executor::Executor() {
  plans_ = {};
  next_ = 0;
}

Executor::Executor(OperNode *plan) : Executor() { plans_.push_back(plan); }

Executor::~Executor() {
  TID tid = std::this_thread::get_id();
  TxManager::GetInstance().Pop(tid);
  for (; next_ < plans_.size(); ++next_) {
    delete plans_[next_];
  }
}

void Executor::AppendPlan(OperNode *plan) { plans_.push_back(plan); }

RecordList Executor::RunNext() {
  if (next_ >= plans_.size()) return {};
  OperNode *plan = plans_[next_];
  auto res = plan->Next();
  delete plan;
  ++next_;
  return res;
}

bool Executor::Finished() const { return next_ >= plans_.size(); }

void Executor::Begin() {
  TID tid = std::this_thread::get_id();
  // 事务开始，获取新的事务ID
  XID xid = TxManager::GetInstance().Push(tid);
  // 记录事务开始日志
  LogManager::GetInstance().Begin(xid);
}

void Executor::Commit() {
  // 事务提交
  // NoForce策略，不会立即写回
  TID tid = std::this_thread::get_id();
  XID xid = TxManager::GetInstance().Get(tid);
  // 记录事务提交日志，WAL
  LogManager::GetInstance().Commit(xid);
  // 清除线程记录
  TxManager::GetInstance().Pop(tid);
}

void Executor::Abort() {
  // 事务中止
  // 调用LogManager::Undo
  TID tid = std::this_thread::get_id();
  XID xid = TxManager::GetInstance().Get(tid);
  // 记录事务中止日志，WAL
  LogManager::GetInstance().Abort(xid);
  // 清除线程记录
  TxManager::GetInstance().Pop(tid);
}

}  // namespace dbtrain
