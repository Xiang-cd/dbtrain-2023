#include "page_handle.h"

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
  slots_ = page->GetData() + sizeof(PageHeader) + meta.bitmap_length_;
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
  SlotID slot_no = bitmap_.FirstFree();
  PageID page_no = page_->GetPageId().page_no;
  RecordFactory RF = RecordFactory(&meta_);
  RF.SetRid(record, {page_no, slot_no});
  RF.StoreRecord(slots_ + record_length_ * slot_no, record);

  bitmap_.Set(slot_no);
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
  int slot_no = -1;
  RecordFactory record_factory(&meta_);
  std::vector<Record *> record_vector;
  while ((slot_no = bitmap_.NextNotFree(slot_no)) != -1) {
    Record *record = record_factory.LoadRecord(slots_ + slot_no * record_length_);
    record_vector.push_back(record);
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

  int slot_no = -1;
  std::vector<Record *> record_vector;
  RecordFactory record_factory(&meta_);
  while ((slot_no = bitmap_.NextNotFree(slot_no)) != -1) {
    Record *record = record_factory.LoadRecord(slots_ + slot_no * record_length_);
    // TODO: MVCC情况下的数据读取
    // TIPS: 注意MVCC在数据读取过程中存在无效数据（未提交的删除以及未开始的插入），注意去除
    // LAB 3 BEGIN
    // LAB 3 END
    record_vector.push_back(record);
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
