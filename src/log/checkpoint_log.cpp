#include "checkpoint_log.h"

#include <cassert>
#include "utils/debug-print.hpp"
#include "tx/tx_manager.h"

namespace dbtrain {

CheckpointLog::CheckpointLog() : Log() {}
CheckpointLog::CheckpointLog(LSN lsn) : Log(lsn) {}
void CheckpointLog::Load(const Byte *src) {
  Log::Load(src);
  size_t offset = sizeof(LSN);
  LogManager &LM = LogManager::GetInstance();
  // TODO: 加载MasterRecord对应的Checkpoint Log
  // TIPS: 利用读取的信息更新LogManager
  // LAB 2 BEGIN
  auto & dpt = LM.dpt_;
  auto & att = LM.att_;
  size_t att_len , dpt_len;
  memcpy(&att_len, src + offset,sizeof(size_t)); offset += sizeof(size_t);
  memcpy(&dpt_len, src + offset,sizeof(size_t)); offset += sizeof(size_t);
  for (int i = 0; i < att_len; ++i) {
    XID xid; LSN lsn;
    memcpy(&xid, src + offset, sizeof(XID)); offset += sizeof(XID);
    memcpy(&lsn, src + offset, sizeof(LSN)); offset += sizeof(LSN);
    att[xid] = lsn;
    Print("CheckpointLog::Load>xid:", xid, " lsn:", lsn);
  }
  for (int i = 0; i < dpt_len; ++i) {
    size_t str_len;
    memcpy(&str_len, src + offset, sizeof(size_t)); offset += sizeof(size_t);
    char array[str_len + 1]; array[str_len] = '\0';
    memcpy(array, src + offset, str_len); offset += str_len;
    string table_name = string(array); PageID page_id; LSN lsn;
    memcpy(&page_id, src + offset, sizeof(PageID)); offset += sizeof(PageID);
    memcpy(&lsn,  src + offset , sizeof(LSN)); offset += sizeof(LSN);
    dpt[{table_name, page_id}] = lsn;

  }

  // LAB 2 END
}

size_t CheckpointLog::Store(Byte *dst) {
  LogManager &LM = LogManager::GetInstance();
  size_t fsize = Log::Store(dst);
  // TODO: 存储LogManager相关信息，返回Store的数据长度
  // TIPS: 不添加缓存机制情况下，仅需要保存ATT和DPT
  // TIPS: 考虑缓存机制情况下，需要额外存储Flushed LSN
  // LAB 2 BEGIN
  const auto & dpt = LM.dpt_;
  const auto & att = LM.att_;
  auto offset = fsize;
  size_t dpt_len = dpt.size();
  size_t att_len = att.size();
  memcpy(dst + offset, &att_len, sizeof(size_t)); offset += sizeof(size_t);
  memcpy(dst + offset, &dpt_len, sizeof(size_t)); offset += sizeof(size_t);
  for (const auto &att_pair : att) {
    Print("CheckpointLog::Store>xid:", att_pair.first, " lsn:", att_pair.second);
    memcpy(dst + offset, &att_pair.first, sizeof(XID)); offset += sizeof(XID);
    memcpy(dst +offset, &att_pair.second, sizeof(LSN)); offset += sizeof(LSN);
  }

  for (const auto &dpt_pair : dpt) {
    // len of name
    size_t str_len = dpt_pair.first.table_name.size();
    memcpy(dst + offset, &str_len, sizeof(size_t)); offset += sizeof(size_t);
    memcpy(dst + offset , dpt_pair.first.table_name.c_str(), str_len); offset += str_len;
    memcpy(dst + offset, &dpt_pair.first.page_id, sizeof (PageID)); offset += sizeof(PageID);
    memcpy(dst + offset , &dpt_pair.second, sizeof(LSN)); offset += sizeof(LSN);
  }
  // LAB 2 END
  return offset;
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
