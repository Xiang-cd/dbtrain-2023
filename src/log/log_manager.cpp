#include "log_manager.h"

#include "log_factory.h"
#include "logs.h"
#include "system/system_manager.h"
#include "tx/tx_manager.h"
#include "utils/debug-print.hpp"

namespace dbtrain {

LogManager &LogManager::GetInstance() {
  static LogManager log_manager;
  return log_manager;
}

LogManager::LogManager() {
  att_ = std::map<XID, LSN>();
  dpt_ = std::map<UniquePageID, LSN>();
  flushed_lsn_ = 0;
  current_lsn_ = INIT_LSN;
  checkpoint_lsn_ = NULL_LSN;
  TxManager::GetInstance().SetXID(INIT_XID);
}

void LogManager::Init() {
  // LogManager参数初始化
  att_.clear();
  dpt_.clear();
  flushed_lsn_ = 0;
  current_lsn_ = INIT_LSN;
  checkpoint_lsn_ = NULL_LSN;
  TxManager::GetInstance().SetXID(INIT_XID);
}

void LogManager::Close() {
  // TIPS: 可以实现缓存机制
  // TIPS: 当使用日志缓存且存在未落盘日志时，先将剩余日志落盘

  // LogManager参数初始化
  flushed_lsn_ = 0;
  current_lsn_ = INIT_LSN;
  att_.clear();
  dpt_.clear();
}

void LogManager::Begin(XID xid) {
  // 记录事务开始日志
  LSN lsn = AppendLog();
  LSN prev_lsn = NULL_LSN;
  Log *log = new BeginLog(lsn, prev_lsn, xid);
  WriteLog(log);
  // 更新ATT
  att_[xid] = lsn;
  delete log;
}

void LogManager::Commit(XID xid) {
  // 记录事务提交日志
  LSN lsn = AppendLog();
  LSN prev_lsn = att_[xid];
  Log *log = new CommitLog(lsn, prev_lsn, xid);
  WriteLog(log);
  // 更新ATT
  att_.erase(xid);
  delete log;
}

void LogManager::Abort(XID xid) {
  // 记录事务中止日志后回滚事务
  // TIPS: 考虑回滚失败的情况需要添加CLR日志
  LSN lsn = AppendLog();
  LSN prev_lsn = att_[xid];
  Log *log = new AbortLog(lsn, prev_lsn, xid);
  WriteLog(log);
  // Undo操作
  Undo(xid);
  // 更新ATT
  att_.erase(xid);
  delete log;
}

void LogManager::Checkpoint() {
  // 记录Checkpoint日志后更新MasterRecord
  LSN lsn = AppendLog();
  Log *log = new CheckpointLog(lsn);
  WriteLog(log);
  delete log;
  SystemManager::GetInstance().StoreMasterRecord();
}

void LogManager::InsertRecordLog(XID xid, const string &table_name, Rid rid, size_t new_len, const void *new_val) {
  // TODO: 记录数据插入日志
  // TIPS: 利用LogFactory生成日志信息
  // TIPS: 注意需要按照算法更新ATT和DPT
  // LAB 2 BEGIN
  Print("insert record log");
  // LAB 2 END
}

void LogManager::DeleteRecordLog(XID xid, const string &table_name, Rid rid, size_t old_len, const void *old_val) {
  // TODO: 记录数据删除日志
  // TIPS: 利用LogFactory生成日志信息
  // TIPS: 注意需要按照算法更新ATT和DPT
  // LAB 2 BEGIN
  // LAB 2 END
}

void LogManager::UpdateRecordLog(XID xid, const string &table_name, Rid rid, size_t old_len, const void *old_val,
                                 size_t new_len, const void *new_val) {
  // TODO: 记录数据更新日志
  // TIPS: 利用LogFactory生成日志信息
  // TIPS: 注意需要按照算法更新ATT和DPT
  // LAB 2 BEGIN
  // LAB 2 END
}

void LogManager::WritePage(int fd, PageID page_id) {
  // 更新DPT
  string table_name = SystemManager::GetInstance().GetTableByFd(fd);
  if (dpt_.find({table_name, page_id}) != dpt_.end()) dpt_.erase({table_name, page_id});
}

LSN LogManager::GetCurrent() const { return current_lsn_ - 1; }

void LogManager::WriteLog(Log *log) {
  // TIPS: 可以在此处暂存部分Log实现缓存机制
  // TIPS: 添加缓存机制时需要保证Commit时日志刷盘
  log_mutex_.lock();
  SystemManager::GetInstance().WriteLog(log);
  log_mutex_.unlock();
}

LSN LogManager::AppendLog() {
  SystemManager::GetInstance().UsingTest();
  lsn_mutex_.lock();
  auto lsn = current_lsn_++;
  lsn_mutex_.unlock();
  return lsn;
}

void LogManager::Analyse(LSN checkpoint_lsn) {
  // 根据ATT和DPT确定需要REDO的XID
  LSN iter_lsn = checkpoint_lsn + 1;
  Log *log = SystemManager::GetInstance().ReadLog(iter_lsn);
  while (log != nullptr) {
    assert(log->GetLSN() == iter_lsn);
    if (log->GetType() == LogType::COMMIT) {
      CommitLog *commit_log = dynamic_cast<CommitLog *>(log);
      XID xid = commit_log->GetXID();
      assert(att_.find(xid) != att_.end());
      att_.erase(xid);
    } else if (log->GetType() == LogType::UPDATE) {
      UpdateLog *update_log = dynamic_cast<UpdateLog *>(log);
      XID xid = update_log->GetXID();
      att_[xid] = log->GetLSN();
      // 更新DPT
      UniquePageID uid = update_log->GetUniPageID();
      if (dpt_.find(uid) == dpt_.end()) dpt_[uid] = log->GetLSN();
    } else if ((log->GetType() == LogType::BEGIN) || (log->GetType() == LogType::ABORT)) {
      TxLog *tx_log = dynamic_cast<TxLog *>(log);
      XID xid = tx_log->GetXID();
      att_[xid] = log->GetLSN();
    } else {
      assert(false);
    }
    delete log;
    ++iter_lsn;
    log = SystemManager::GetInstance().ReadLog(iter_lsn);
  }
  checkpoint_lsn_ = checkpoint_lsn;
  current_lsn_ = iter_lsn;
}

void LogManager::Redo() {
  // REDO过程
  // TIPS: 按照ARIES算法，需要读取DPT获取最小的Record LSN
  // TIPS: 从最小RecLSN开始REDO，根据PageLSN部分数据不需要REDO
  // LAB 2 BEGIN
  // LAB 2 END
}

void LogManager::Undo() {
  // 在目前实现方法下，所有处于ATT中的事务都为正在运行状态
  // Undo过程需要回滚所有的ATT表中事务
  for (const auto &att_pair : att_) {
    Undo(att_pair.first);
    // 清除ATT表对应项
  }
  att_.clear();
}

bool LogManager::Undo(XID xid) {
  // TODO: 回滚事务
  // TIPS: 按照ATT表中事务最后更改，进行回滚
  // TIPS: 每次读取Previous LSN读取之前的日志
  // TIPS: 直到事务开始时停止回滚过程
  // TIPS: 利用Update Log的Undo功能可以实现数据恢复
  // TIPS: 基础功能下不需要考虑回滚过程中CRASH
  // LAB 2 BEGIN
  // LAB 2 END
}

}  // namespace dbtrain
