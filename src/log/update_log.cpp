#include "update_log.h"

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
  // LAB 2 END
}

void UpdateLog::Undo() {
  // TODO: 根据Image进行Undo操作
  // TIPS: 可以直接基于Image定位页面并按照操作类型进行Undo
  // TIPS: 基础功能不需要考虑Meta信息和页头信息的变化
  // LAB 2 BEGIN
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

}  // namespace dbtrain
