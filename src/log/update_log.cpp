#include "update_log.h"
#include "utils/debug-print.hpp"
#include "log_image.h"
#include "system/system_manager.h"

namespace dbtrain {

typedef PhysiologicalImage LogImage;

UpdateLog::UpdateLog(LSN lsn, LSN prev_lsn, XID xid) : TxLog(lsn, prev_lsn, xid) {}

void UpdateLog::Redo() {
  // TODO: 根据Image进行Redo操作
  // TIPS: 需要先结合PageLSN判断是否需要进行Redo
  // TIPS: 可以直接基于Image定位页面并按照操作类型进行Redo
  // TIPS: 基础功能不需要考虑Meta信息和页头信息的变化
  // LAB 2 BEGIN
  UniquePageID upid = GetUniPageID();
  Table *table = SystemManager::GetInstance().GetTable(upid.table_name);
  PageHandle PH = table->GetPage(upid.page_id);
  if (PH.GetLSN() >= lsn_){
    Print("UpdateLog::Redo> not need to redo page lsn:", PH.GetLSN(), " lsn:", lsn_);
    return;
  } else {
    auto type = log_image_.op_type_;
    auto lsn = GetLSN();
    if (type == LogOpType::INSERT){
      PH.InsertRecord(log_image_.slot_id_, log_image_.new_val_, lsn);
    } else if (type == LogOpType::DELETE){
      PH.DeleteRecord(log_image_.slot_id_, lsn);
    } else if (type == LogOpType::UPDATE){
      PH.UpdateRecord(log_image_.slot_id_, log_image_.new_val_, lsn);
    }else assert(false);
  }
  // LAB 2 END
}

void UpdateLog::Undo(LSN lsn) {
  // TODO: 根据Image进行Undo操作
  // TIPS: 可以直接基于Image定位页面并按照操作类型进行Undo
  // TIPS: 基础功能不需要考虑Meta信息和页头信息的变化
  // LAB 2 BEGIN
  UniquePageID upid = GetUniPageID();
  Table *table = SystemManager::GetInstance().GetTable(upid.table_name);
  PageHandle PH = table->GetPage(upid.page_id);
  Print("UpdateLog::Undo()>",upid.table_name, " page_id:", upid.page_id, " page lsn", PH.GetLSN(), " lsn:", lsn_);

  LogOpType type = log_image_.op_type_;
  if (type == LogOpType::INSERT){
    PH.DeleteRecord(log_image_.slot_id_, lsn);
  } else if (type == LogOpType::DELETE){
    PH.InsertRecord(log_image_.slot_id_, log_image_.old_val_, lsn);
  } else if (type == LogOpType::UPDATE){
    PH.UpdateRecord(log_image_.slot_id_, log_image_.old_val_, lsn);
  }else assert(false);

  // LAB 2 END
}

void UpdateLog::Load(const Byte *src) {
  TxLog::Load(src);
  auto src_ = src + 2 * sizeof(LSN) + sizeof(XID);
  log_image_.Load(src_);
}

size_t UpdateLog::Store(Byte *dst) {
  size_t length = TxLog::Store(dst);
  auto dst_ = dst + length;
  size_t img_length = log_image_.Store(dst_);
  return length + img_length;
}

UniquePageID UpdateLog::GetUniPageID() const { return {log_image_.table_name_, log_image_.page_id_}; }

LogType UpdateLog::GetType() const { return LogType::UPDATE; }

size_t UpdateLog::GetLength() const { return TxLog::GetLength() + log_image_.GetLength(); }



CLRLog::CLRLog(LSN lsn, LSN prev_lsn, XID xid, LSN undo) : UpdateLog(lsn, prev_lsn, xid) {undoNext = undo;}
void CLRLog::Load(const Byte *src) {
    memcpy(&undoNext, src, sizeof(LSN));
    UpdateLog::Load(src + sizeof(LSN));
};
size_t CLRLog::Store(Byte *dst) {
  memcpy(dst, &undoNext, sizeof(LSN));
  size_t length = UpdateLog::Store(dst + sizeof(LSN));
  return length + sizeof(LSN);
};
LogType CLRLog::GetType() const { return LogType::CLR; }
size_t  CLRLog::GetLength() const { return UpdateLog::GetLength() + sizeof(undoNext); };
void CLRLog::from_UpdateLog(const UpdateLog * ulog){
  const auto & src = ulog->log_image_;
  log_image_.slot_id_ = src.slot_id_;
  log_image_.table_name_ = src.table_name_;
  log_image_.page_id_ = src.page_id_;


  if (src.op_type_ == LogOpType::INSERT){
    log_image_.op_type_ = LogOpType::DELETE;
    auto len = log_image_.old_len_ = src.new_len_;
    log_image_.old_val_ = new Byte [len];
    memcpy(log_image_.old_val_, src.new_val_, len);
  }else if (src.op_type_ == LogOpType::DELETE){
    log_image_.op_type_ = LogOpType::INSERT;
    auto len = log_image_.new_len_ = src.old_len_;
    log_image_.new_val_ = new Byte [len];
    memcpy(log_image_.new_val_, src.old_val_, len);
  }else if (src.op_type_ == LogOpType::UPDATE){
    log_image_.op_type_ = LogOpType::UPDATE;
    auto len = log_image_.old_len_ = src.new_len_;
    log_image_.old_val_ = new Byte [len];
    memcpy(log_image_.old_val_, src.new_val_, len);

    len = log_image_.new_len_ = src.old_len_;
    log_image_.new_val_ = new Byte [len];
    memcpy(log_image_.new_val_, src.old_val_, len);
  }
}


}  // namespace dbtrain
