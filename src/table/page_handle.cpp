#include "page_handle.h"
#include "defines.h"
#include <cassert>
#include <set>
#include <algorithm>
#include "utils/debug-print.hpp"
#include "record/record_factory.h"
#include "tx/lock_manager.h"

namespace dbtrain {

PageHandle::PageHandle(Page *page, const TableMeta &meta)
    : page_(page),
      record_length_(meta.record_length_),
      bitmap_(page->GetData() + sizeof(PageHeader), meta.record_per_page_),
      meta_(meta) {
  header_ = (PageHeader *)page->GetData();
  if (meta_.is_var){
    slots_ = nullptr;
    positions = (Position *)(page->GetData() + sizeof(PageHeader));
    for (int i = 0; i < header_->num_record; ++i) {
      if (not positions[i].valid) empty_slots.push_back(i);
    }
    Print("num empty slot:", empty_slots.size());
  }else{
    slots_ = page->GetData() + sizeof(PageHeader) + meta.bitmap_length_;
    positions = nullptr;
  }

}

int PageHandle::QuerySize(int size_needed){
  // 遍历空的slot, 如果比较大, 就返回slot num,
  // 如果没有空的slot,则尝试末尾添加, 检查block size
  int num_empty_slot = empty_slots.size();
  for (int i = 0; i < num_empty_slot; ++i) {
    const Position &p = positions[i];
    if (not p.valid and p.len >= size_needed)
      return i;
  }
  if (BlockSpace() >= sizeof(Position) + size_needed)
    return header_->num_record;
  return -1;
}

void PageHandle::InsertRecord(Record *record, SlotID slot_no, int store_len) {
  assert(meta_.is_var);
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));
  PageID page_no = page_->GetPageId().page_no;
  RecordFactory RF = RecordFactory(&meta_);
  Position &p = positions[slot_no];
  RF.SetRid(record, {page_no, slot_no});
  if (slot_no == header_->num_record){
    header_->offset_of_free -= store_len;
    p.offset = header_->offset_of_free;
    p.len = p.data_len = store_len;
    p.valid = true;
    header_->num_record ++;
  }else{
    p.valid = true;
    assert(p.len >= store_len);
    p.data_len = store_len;
    header_->pieces_space += (p.len - p.data_len);
  }
  RF.StoreRecord(page_->GetData() + p.offset, record);
  page_->SetDirty();
  Print("record start offset:", p.offset, " record len:", p.len, "data len:", p.data_len);
  Print("record store len:", store_len);

  SetLSN(LogManager::GetInstance().GetCurrent());
  // 释放排它锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::InsertRecord(Record *record) {
  assert(not meta_.is_var);
  // 获取排它锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));
  // TODO: 插入记录
  // TIPS: 通过bitmap_.FirstFree()获取第一个空槽
  // TIPS: 使用RecordFactory::SetRid设置record的rid
  // TIPS: 使用RecordFactory的StoreRecord方法将record序列化到页面中
  // TIPS: 将bitmap_的第一个空槽标记为已使用
  // TIPS: 将page_标记为dirty
  // LAB 1 BEGIN
  SlotID slot_no = bitmap_.FirstFree();
  PageID page_no = page_->GetPageId().page_no;
  RecordFactory RF = RecordFactory(&meta_);
  RF.SetRid(record, {page_no, slot_no});
  RF.StoreRecord(slots_ + record_length_ * slot_no, record);
  bitmap_.Set(slot_no);
  header_->num_record ++;
  page_->SetDirty();

  // LAB 1 END
  // LAB 2: 设置页面LSN
  SetLSN(LogManager::GetInstance().GetCurrent());
  // 释放排它锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::DeleteRecord(SlotID slot_no) {
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));
  // TODO: 删除记录
  // TIPS: 直接设置bitmap_为0即可删除对应记录
  // TIPS: 将page_标记为dirty
  // LAB 1 BEGIN
  if (meta_.is_var){
    Print("delete slot no:", slot_no, " total_num:", header_->num_record);
    assert(slot_no < header_->num_record);
    positions[slot_no].valid = false;
    empty_slots.push_back(slot_no);
//    Print("offset:", P.offset, " len:", P.len);
//    int behind_total_len = 0;
//    std::vector<Record *> record_vector;
//    RecordFactory record_factory(&meta_);
//    for (int i = slot_no + 1; i < header_->num_record; ++i) {
//      behind_total_len += positions[i].len;
//      Position p = positions[i];
//      uint8_t * data_start = page_->GetData() + p.offset;
//      Print("slot no:", i , " offset:", p.offset);
//      Record * record = record_factory.LoadRecord(data_start);
//      Rid rid = record_factory.GetRid(record);
//      rid.slot_no --;
//      record_factory.SetRid(record, rid);
//      record_vector.push_back(record);
//    }
//    header_->num_record--;
//    for (int i = slot_no; i < header_->num_record; ++i) {
//      positions[i].len = positions[i+1].len;
//      positions[i].offset = positions[i+1].offset + P.len;
//    }
//    uint8_t array[behind_total_len];
//    memcpy(array, page_->GetData() + header_->offset_of_free, behind_total_len);
//    header_->offset_of_free += P.len;
//    for (int i = slot_no, j=0; i < header_->num_record; ++i, ++j) {
//      record_factory.StoreRecord(page_->GetData()+positions[i].offset, record_vector[j]);
//    }
//    memcpy(page_->GetData() + header_->offset_of_free, array, behind_total_len);
//    Print("behind total len:", behind_total_len);
  }else{
    bitmap_.Reset(slot_no);
    header_->num_record--;

  }
  page_->SetDirty();
  // LAB 1 END
  // LAB 2: 设置页面LSN
  SetLSN(LogManager::GetInstance().GetCurrent());
  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::UpdateRecord(SlotID slot_no, Record *record) {
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));
  // TODO: 更新记录
  // TIPS: 由于使用了定长数据管理，可以利用新的record序列化结果覆盖对应页面数据
  // TIPS: 将page_标记为dirty
  // LAB 1 BEGIN
  RecordFactory RF = RecordFactory(&meta_);
  RF.StoreRecord(slots_ + record_length_ * slot_no, record);
  page_->SetDirty();
  // LAB 1 END
  // 设置页面LSN
  SetLSN(LogManager::GetInstance().GetCurrent());
  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::UpdateRecord(SlotID slot_no, Record *record, int store_len) {
  assert(not meta_.is_var);
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));
  // TODO: 更新记录
  // TIPS: 由于使用了定长数据管理，可以利用新的record序列化结果覆盖对应页面数据
  // TIPS: 将page_标记为dirty
  // LAB 1 BEGIN
  PageID page_no = page_->GetPageId().page_no;
  RecordFactory RF = RecordFactory(&meta_);
  RF.SetRid(record, {page_no, slot_no});

  Position &p = positions[slot_no];
  p.valid = true;
  p.data_len = store_len;
  header_->pieces_space += p.len - p.data_len; // 产生碎片
  std::remove(empty_slots.begin(), empty_slots.end(), slot_no);

  RF.StoreRecord(page_->GetData() + p.offset, record);
  page_->SetDirty();
  // LAB 1 END
  // 设置页面LSN
  SetLSN(LogManager::GetInstance().GetCurrent());
  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

RecordList PageHandle::LoadRecords() {
  // 获取共享锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.LockShared("Page" + std::to_string(page_->GetPageId().page_no));


  std::vector<Record *> record_vector;
  RecordFactory record_factory(&meta_);
  Print("this page have", header_->num_record, " records");
  int num_invalid = 0;
  if (meta_.is_var){
    for (int i = 0; i < header_->num_record; ++i) {
      Position p = positions[i];
      if (not p.valid){
        num_invalid ++;
        continue;
      }
      uint8_t * data_start = page_->GetData() + p.offset;
      Record * record = record_factory.LoadRecord(data_start);
      record_vector.push_back(record);
    }
  }else{
    int slot_no = -1;
    while ((slot_no = bitmap_.NextNotFree(slot_no)) != -1) {
      Record *record = record_factory.LoadRecord(slots_ + slot_no * record_length_);
      record_vector.push_back(record);
    }
  }
  Print("this page have", num_invalid, "invalid records");

  // 释放共享锁
  lock_manager.UnlockShared("Page" + std::to_string(page_->GetPageId().page_no));
  return record_vector;
}

uint8_t *PageHandle::GetRaw(SlotID slot_no) { return slots_ + slot_no * record_length_; }

Record *PageHandle::GetRecord(SlotID slot_no) {
  // 获取共享锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.LockShared("Page" + std::to_string(page_->GetPageId().page_no));

  if (bitmap_.Test(slot_no)) {
    RecordFactory record_factory(&meta_);
    Record *record = record_factory.LoadRecord(slots_ + slot_no * record_length_);
    // 释放共享锁
    lock_manager.UnlockShared("Page" + std::to_string(page_->GetPageId().page_no));
    return record;
  } else {
    // 释放共享锁
    lock_manager.UnlockShared("Page" + std::to_string(page_->GetPageId().page_no));
    return nullptr;
  }
}

void PageHandle::InsertRecord(SlotID slot_no, const void *src, LSN lsn) {
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));

  bitmap_.Set(slot_no);
  memcpy(slots_ + slot_no * record_length_, src, meta_.GetLength());
  page_->SetDirty();
  // 设置页面LSN
  SetLSN(lsn);

  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::DeleteRecord(SlotID slot_no, LSN lsn) {
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));

  bitmap_.Reset(slot_no);
  page_->SetDirty();
  // 设置页面LSN
  SetLSN(lsn);

  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::UpdateRecord(SlotID slot_no, const void *src, LSN lsn) {
  // 获取排他锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.Lock("Page" + std::to_string(page_->GetPageId().page_no));

  assert(bitmap_.Test(slot_no));
  memcpy(slots_ + slot_no * record_length_, src, meta_.GetLength());
  page_->SetDirty();
  // 设置页面LSN
  SetLSN(lsn);

  // 释放排他锁
  lock_manager.Unlock("Page" + std::to_string(page_->GetPageId().page_no));
}

void PageHandle::InsertRecord(Record *record, XID xid) {
  // TODO: MVCC情况下的数据插入
  // TIPS: 注意需要利用锁保证页面仅能同时被单个线程修改
  // TIPS: 注意MVCC需要设置版本号，版本号可以用事务号表示
  // LAB 3 BEGIN
  // LAB 3 END
}

void PageHandle::DeleteRecord(SlotID slot_no, XID xid, bool) {
  // TODO: MVCC情况下的数据删除
  // TIPS: 注意需要利用锁保证页面仅能同时被单个线程修改
  // TIPS: 注意MVCC删除不能直接清除数据，只是设置对应记录失效
  // LAB 3 BEGIN
  // LAB 3 END
}

RecordList PageHandle::LoadRecords(XID xid, const std::set<XID> &uncommit_xids) {
  // 获取共享锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.LockShared("Page" + std::to_string(page_->GetPageId().page_no));
  // 生成判定集合
  assert(uncommit_xids.find(xid) == uncommit_xids.end());

  std::vector<Record *> record_vector;
  RecordFactory record_factory(&meta_);
  Print("this page have", header_->num_record, " records");
  if (meta_.is_var){
    for (int i = 0; i < header_->num_record; ++i) {
      Position p = positions[i];
      if (not p.valid){
        continue;
      }
      uint8_t * data_start = page_->GetData() + p.offset;
      Print("slot no:", i , " offset:", p.offset);
      Record * record = record_factory.LoadRecord(data_start);
      record_vector.push_back(record);
    }
  }else{
    int slot_no = -1;
    while ((slot_no = bitmap_.NextNotFree(slot_no)) != -1) {
      Record *record = record_factory.LoadRecord(slots_ + slot_no * record_length_);
      // TODO: MVCC情况下的数据读取
      // TIPS: 注意MVCC在数据读取过程中存在无效数据（未提交的删除以及未开始的插入），注意去除
      // LAB 3 BEGIN
      // LAB 3 END
      record_vector.push_back(record);
    }
  }

  // 释放共享锁
  lock_manager.UnlockShared("Page" + std::to_string(page_->GetPageId().page_no));
  return record_vector;
}

Rid PageHandle::Next() { return Rid{0, 0}; }

int PageHandle::BlockSpace() {
  // return space of whole block
  return header_->offset_of_free - (sizeof (PageHeader) + header_->num_record * sizeof (Position));
}



bool PageHandle::Full() {return bitmap_.Full(); }

PageID PageHandle::GetNextFree() { return header_->next_free; }

SlotID PageHandle::FirstFree() { return bitmap_.FirstFree(); }

void PageHandle::SetLSN(LSN lsn) { header_->page_lsn = lsn; }

LSN PageHandle::GetLSN() { return header_->page_lsn; }

}  // namespace dbtrain
