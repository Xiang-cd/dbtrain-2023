#include "page_handle.h"
#include "defines.h"
#include <cassert>
#include <set>
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
  Print("max record len:", record_length_);
  if (meta_.is_var){
    slots_ = nullptr;
    positions = (Position *)(page->GetData() + sizeof(PageHeader));
  }else{
    slots_ = page->GetData() + sizeof(PageHeader) + meta.bitmap_length_;
    positions = nullptr;
  }

}

void PageHandle::InsertRecord(Record *record) {
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
  if (meta_.is_var){
    SlotID slot_no = header_->num_record;
    PageID page_no = page_->GetPageId().page_no;
    // getting actual record len
    int num_field = record->GetSize();
    int actual_record_len = 0; // 在磁盘层面的长度, 所以需要加上varchar记录长度的int
    for (int i = 0; i < num_field; ++i) {
      if (record->field_list_[i]->GetType() == FieldType::VCHAR){
        actual_record_len += record->field_list_[i]->GetSize() + sizeof(int);
        Print("varchar actual len:", record->field_list_[i]->GetSize(), " store len:", record->field_list_[i]->GetSize() + sizeof(int));
      }else{
        actual_record_len += meta_.cols_[i].len_;
      }
      Print("type:", type2str[meta_.cols_[i].type_], " len:", meta_.cols_[i].len_);
    }
    Print("actual len:", actual_record_len);
    Print("slot_no:", slot_no);
    header_->offset_of_free -= actual_record_len;
    Position &p = positions[slot_no];
    p.offset = header_->offset_of_free;
    p.len = actual_record_len;
    Print("record start offset:", p.offset, " recored len:", p.len);
    RecordFactory RF = RecordFactory(&meta_);
    RF.SetRid(record, {page_no, slot_no});
    RF.StoreRecord(page_->GetData() + p.offset, record);
    header_->num_record ++;
  }else{
    SlotID slot_no = bitmap_.FirstFree();
    PageID page_no = page_->GetPageId().page_no;
    RecordFactory RF = RecordFactory(&meta_);
    RF.SetRid(record, {page_no, slot_no});
    RF.StoreRecord(slots_ + record_length_ * slot_no, record);
    bitmap_.Set(slot_no);
    header_->num_record ++;
  }
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
  bitmap_.Reset(slot_no);
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

RecordList PageHandle::LoadRecords() {
  // 获取共享锁
  LockManager &lock_manager = LockManager::GetInstance();
  lock_manager.LockShared("Page" + std::to_string(page_->GetPageId().page_no));


  std::vector<Record *> record_vector;
  RecordFactory record_factory(&meta_);
  Print("this page have", header_->num_record, " records");
  if (meta_.is_var){
    for (int i = 0; i < header_->num_record; ++i) {
      Position p = positions[i];
      uint8_t * data_start = page_->GetData() + p.offset;
      Print("slot no:", i , " offset:", p.offset);
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

bool PageHandle::Full() { return bitmap_.Full(); }

PageID PageHandle::GetNextFree() { return header_->next_free; }

SlotID PageHandle::FirstFree() { return bitmap_.FirstFree(); }

void PageHandle::SetLSN(LSN lsn) { header_->page_lsn = lsn; }

LSN PageHandle::GetLSN() { return header_->page_lsn; }

}  // namespace dbtrain
