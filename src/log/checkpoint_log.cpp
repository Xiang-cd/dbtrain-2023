#include "checkpoint_log.h"

#include <cassert>

#include "tx/tx_manager.h"

namespace dbtrain {

CheckpointLog::CheckpointLog() : Log() {}
CheckpointLog::CheckpointLog(LSN lsn) : Log(lsn) {}
void CheckpointLog::Load(const Byte *src) {
  Log::Load(src);
  LogManager &log_manager = LogManager::GetInstance();
  // TODO: 恢复当前事务编号
  // LAB 3 BEGIN
  // LAB 3 END
  // TODO: 加载MasterRecord对应的Checkpoint Log
  // TIPS: 利用读取的信息更新LogManager
  // LAB 2 BEGIN
  // LAB 2 END
}

size_t CheckpointLog::Store(Byte *dst) {
  LogManager &log_manager = LogManager::GetInstance();
  size_t fsize = Log::Store(dst);
  // TODO: 存储当前事务编号
  // LAB 3 BEGIN
  // LAB 3 END
  // TODO: 存储LogManager相关信息，返回Store的数据长度
  // TIPS: 不添加缓存机制情况下，仅需要保存ATT和DPT
  // TIPS: 考虑缓存机制情况下，需要额外存储Flushed LSN
  // LAB 2 BEGIN
  // LAB 2 END
  return fsize;
}
LogType CheckpointLog::GetType() const { return LogType::CHECKPOINT; }

size_t CheckpointLog::GetLength() const {
  size_t length = Log::GetLength() + 2 * sizeof(LSN) + sizeof(XID);
  length += 2 * sizeof(size_t);
  LogManager &log_manager = LogManager::GetInstance();
  length += log_manager.att_.size() * (sizeof(XID) + sizeof(LSN));
  length += log_manager.dpt_.size() * (sizeof(LSN) + sizeof(PageID) + sizeof(size_t));
  for (const auto &dpt_pair : log_manager.dpt_) {
    length += dpt_pair.first.table_name.size();
  }
  return length;
}

}  // namespace dbtrain
