#ifndef DBTRAIN_PAGE_HANDLE_H
#define DBTRAIN_PAGE_HANDLE_H

#include <set>

#include "defines.h"
#include "storage/buffer_manager.h"
#include "storage/page.h"
#include "table/table_meta.h"
#include "utils/bitmap.h"

namespace dbtrain {

// TIPS: 可自行添加字段
struct PageHeader {
  LSN page_lsn;
  PageID next_free;
  SlotID num_record;
  int pieces_space;
  int offset_of_free;
};

struct Position {
  // 变长记录的长度, 变长记录所在的偏移量
  int len;
  int data_len;
  int offset;
  bool valid;
};

class PageHandle {
  friend class Table;

 public:
  PageHandle() = default;
  PageHandle(Page *page, const TableMeta &meta);
  ~PageHandle() = default;

  // 无并发接口
  void InsertRecord(Record *record);
  void DeleteRecord(SlotID slot_no);
  void UpdateRecord(SlotID slot_no, Record *record);
  RecordList LoadRecords();

  // LAB 2: 新增部分函数方便后续实验
  uint8_t *GetRaw(SlotID slot_no);
  Record *GetRecord(SlotID slot_no);

  void InsertRecord(SlotID slot_no, const void *data, LSN lsn);
  void DeleteRecord(SlotID slot_no, LSN lsn);
  void UpdateRecord(SlotID slot_no, const void *data, LSN lsn);

  void InsertRecord(Record *record, SlotID slot_no, int store_len);
  void UpdateRecord(SlotID slot_no, Record *record, int store_len);
    // LAB 3: MVCC接口
  void InsertRecord(Record *record, XID xid);
  // 第三个参数没有实际含义，仅用于区分重载函数 DeleteRecord(SlotID slot_no, LSN lsn)
  void DeleteRecord(SlotID, XID xid, bool);
  RecordList LoadRecords(XID xid, const std::set<XID> &uncommit_xids);

  Rid Next();
  int BlockSpace();
  bool Full();
  PageID GetNextFree();
  SlotID FirstFree();
  void SetLSN(LSN lsn);
  LSN GetLSN();

  std::vector<SlotID> empty_slots; // varchar page use only
  int QuerySize(int size_needed);
 private:
  Bitmap bitmap_;
  PageHeader *header_;
  int record_length_;
  uint8_t *slots_;
  Position * positions;
  Page *page_;
  TableMeta meta_;
};

}  // namespace dbtrain

#endif
