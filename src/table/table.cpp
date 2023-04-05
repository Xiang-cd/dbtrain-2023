#include "table.h"

#include <algorithm>

#include "exception/exceptions.h"
#include "exception/table_exceptions.h"
#include "log/log_manager.h"
#include "record/fields.h"
#include "record/record_factory.h"
#include "result/result.h"
#include "table/hidden.h"
#include "tx/tx_manager.h"
#include "utils/debug-print.hpp"

namespace dbtrain {

Table::Table(const std::string &table_name, int meta_fd, int data_fd)
    : table_name_(table_name), meta_fd_(meta_fd), data_fd_(data_fd), buffer_manager_(BufferManager::GetInstance()) {
  Page *meta_page = buffer_manager_.GetPage(meta_fd_, META_PAGE_NO);
  meta_.Load(meta_page->GetData());
}

Table::Table(const std::string &table_name, int meta_fd, int data_fd, const std::vector<Column> &columns)
    : table_name_(table_name), meta_fd_(meta_fd), data_fd_(data_fd), buffer_manager_(BufferManager::GetInstance()) {
  meta_.record_length_ = 0;
  for (auto &col : columns) {
    meta_.record_length_ += col.len_;
    meta_.cols_.push_back(col);
  }
  // 元信息添加隐藏列
  for (const auto &col : hidden_columns) {
    meta_.record_length_ += col.len_;
    meta_.cols_.push_back(col);
  }

  meta_.record_per_page_ = (BITMAP_WIDTH * (PAGE_SIZE - sizeof(PageHeader)) - (BITMAP_WIDTH - 1)) /
                           (1 + BITMAP_WIDTH * meta_.record_length_);
  meta_.table_end_page_ = 0;
  meta_.first_free_ = NULL_PAGE;
  meta_.bitmap_length_ = (meta_.record_per_page_ + BITMAP_WIDTH - 1) / BITMAP_WIDTH;

  Page *meta_page = buffer_manager_.AllocPage(meta_fd_, META_PAGE_NO);
  Store(meta_page->GetData());
  meta_page->SetDirty();
}

void Table::StoreMeta() {
  if (meta_modified) {
    Page *meta_page = buffer_manager_.GetPage(meta_fd_, META_PAGE_NO);
    Store(meta_page->GetData());
    meta_page->SetDirty();
  }
}

Table::~Table() {
  if (meta_modified) {
    Page *meta_page = buffer_manager_.GetPage(meta_fd_, META_PAGE_NO);
    Store(meta_page->GetData());
    meta_page->SetDirty();
  }
//  Print("table", table_name_," flushiing file");
  buffer_manager_.FlushFile(meta_fd_);
//  Print("table", table_name_," flushiing meta");
  buffer_manager_.FlushFile(data_fd_);
}

Result Table::Desc() {
  RecordList records;
  for (auto &col : meta_.cols_) {
    if (!IsHiddenColumn(col.name_)) {
      Record *record = new Record();
      record->PushBack(new StrField(col.name_.c_str(), col.name_.size()));
      record->PushBack(new StrField(type2str[col.type_].c_str(), type2str[col.type_].size()));
      record->PushBack(new StrField(std::to_string(col.len_).c_str(), std::to_string(col.len_).size()));
      records.push_back(record);
    }
  }
  return Result({"Name", "Type", "Length"}, records);
}

int Table::Load(const uint8_t *src) { return meta_.Load(src); }

int Table::Store(uint8_t *dst) { return meta_.Store(dst); }

PageHandle Table::CreatePage() {
  Page *page = buffer_manager_.AllocPage(data_fd_, meta_.table_end_page_);
  meta_.first_free_ = meta_.table_end_page_;
  meta_.table_end_page_++;
  meta_modified = true;
  PageHandle page_handle = PageHandle(page, meta_);
  page_handle.header_->next_free = NULL_PAGE;
  page_handle.bitmap_.Init();
  return page_handle;
}

PageHandle Table::GetPage(PageID page_id) {
  Page *page = buffer_manager_.GetPage(data_fd_, page_id);
  PageHandle page_handle = PageHandle(page, meta_);
  return page_handle;
}

void Table::InsertRecord(Record *record) {
  if (record->GetNumField() != meta_.cols_.size()) {
    throw InvalidInsertCountError(record->GetNumField(), meta_.cols_.size());
  }
  for (size_t i = 0; i < record->GetNumField(); i++) {
    if (meta_.cols_[i].type_ != record->GetField(i)->GetType()) {
      throw InvalidInsertTypeError(type2str[record->GetField(i)->GetType()], type2str[meta_.cols_[i].type_]);
    }
    if (meta_.cols_[i].type_ == FieldType::STRING && meta_.cols_[i].len_ < record->GetField(i)->GetSize()) {
      throw InvalidInsertStringLenError(record->GetField(i)->GetSize(), meta_.cols_[i].len_);
    }
  }

  // TODO: 添加数据插入日志信息
  // TIPS: 注意此处需要添加到下面LAB 1添加代码之间
  // TIPS: 注意ARIES使用的是WAL，所以需要先写入日志，再更新数据
  // TIPS: 注意记录日志时需要考虑隐藏列的内容
  // TIPS: 利用LogManager对应函数记录日志
  // TIPS: 需要在LAB 1之间添加代码
  // LAB 2 BEGIN
  // LAB 2 END
  XID xid = TxManager::GetInstance().Get(std::this_thread::get_id());
  LogManager &LM = LogManager::GetInstance();


  // TODO: 更改LAB 1,2代码，适应MVCC情景
  // TIPS: 注意记录日志时需要设置新的隐藏列
  // LAB 3 BEGIN
  // LAB 3 END

  // TODO: 寻找有空页面并插入记录
  // TIPS: 判断meta_.first_free_变量是否为NULL_PAGE
  // TIPS: 若为NULL_PAGE，则调用CreatePage()创建一个新的页面
  // TIPS: 若不为NULL_PAGE，则调用GetPage()获取meta_.first_free_页面
  // TIPS: 调用page_handler的InsertRecord()方法插入记录
  // TIPS: 若当前页面已满，则将meta_.first_free_设为下一个有空位的页面，同时将meta_modified设为true
  // LAB 1 BEGIN
  PageHandle PH;
  if (meta_.first_free_ == NULL_PAGE)
    PH = CreatePage();
  else
    PH = GetPage(meta_.first_free_);


  // 这里会有并发问题, 如果在这里获得bitmap的第一个槽, 其他线程也在此获得一个槽, 则可能冲突
  // 这里采用加锁的方式获得槽, 同时设置槽为不可用
  SlotID slot_no = PH.AtomicSeekAndSet();
  PageID page_no = PH.page_->GetPageId().page_no;

  // 保存记录到buffer, insert 和 log 都采用直接拷贝的方式
  Byte record_raw[meta_.record_length_];
  RecordFactory RF = RecordFactory(&meta_);
  RF.SetRid(record, {page_no, slot_no});
  RF.StoreRecord(record_raw, record);

  LSN lsn = LM.InsertRecordLog(xid, table_name_, {page_no, slot_no}, meta_.record_length_, record_raw);
  PH.InsertRecord(slot_no, record_raw, lsn);


  if (PH.Full()){
    // 主动寻找最近的空页
    PageID cur_page_no = PH.page_->GetPageId().page_no;
    PageID next_free = NULL_PAGE;
    for (PageID i =  cur_page_no + 1; i < meta_.table_end_page_; ++i) {
      if (!GetPage(i).Full()) {
        next_free = i;
        break;
      }
    }
    meta_.first_free_ = next_free;
    meta_modified = true;
  }

  // LAB 1 END
}

void Table::DeleteRecord(const Rid &rid) {
  // TODO: 添加数据删除日志信息
  // TIPS: 注意ARIES使用的是WAL，所以需要先写入日志，再更新数据
  // TIPS: 利用LogManager对应函数记录日志
  // LAB 2 BEGIN
  // LAB 2 END
  XID xid = TxManager::GetInstance().Get(std::this_thread::get_id());
  LogManager &LM = LogManager::GetInstance();

  // TODO: 更改LAB 1,2代码，适应MVCC情景
  // TIPS: 注意删除日志没有清除实际数据，页面不会由满变空
  // LAB 3 BEGIN
  // LAB 3 END

  // TODO: 删除RID对应的记录
  // TIPS: 利用PageID查找对应的页面，通过PageHandle解析页面
  // TIPS: 利用DeleteRecord删除对应SlotID的记录
  // TIPS: 注意更新Meta的first_free_信息
  // LAB 1 BEGIN
  PageHandle PH = GetPage(rid.page_no);
  LM.DeleteRecordLog(xid, table_name_, rid, meta_.record_length_, PH.GetRaw(rid.slot_no));
  PH.DeleteRecord(rid.slot_no);

  if (rid.page_no < meta_.first_free_){
    meta_.first_free_ = rid.page_no;
    meta_modified = true;
  }

  // LAB 1 END
}

void Table::UpdateRecord(const Rid &rid, Record *record) {
  // TODO: 添加数据更新日志信息
  // TIPS: 注意ARIES使用的是WAL，所以需要先写入日志，再更新数据
  // TIPS: 利用LogManager对应函数记录日志
  // LAB 2 BEGIN
  // LAB 2 END
  XID xid = TxManager::GetInstance().Get(std::this_thread::get_id());
  LogManager &LM = LogManager::GetInstance();
  // TODO: 更改LAB 1,2代码，适应MVCC情景
  // TIPS: 注意更新过程与之前不同，需要采用删除旧数据并插入新数据的方法
  // TIPS: 可以调用DeleteRecord和InsertRecord的接口
  // LAB 3 BEGIN
  // LAB 3 END

  // TODO: 更新记录
  // TIPS: 利用PageID查找对应的页面，通过PageHandle解析页面
  // TIPS: 利用UpdateRecord更新对应SlotID的记录为record
  // LAB 1 BEGIN

  // 依然有并发问题, 如果直接对get Raw 做更新, 当别的线程拿到所进行修改, 而这边直接getraw
  Byte record_raw[meta_.record_length_];
  RecordFactory RF = RecordFactory(&meta_);
  RF.StoreRecord(record_raw, record);
  PageHandle PH = GetPage(rid.page_no);

  LM.UpdateRecordLog(xid, table_name_, rid, meta_.record_length_, PH.GetRaw(rid.slot_no), meta_.record_length_, record_raw);

  PH.UpdateRecord(rid.slot_no, record);
  // LAB 1 END
}

int Table::GetColumnSize() const { return meta_.GetSize(); }

TableMeta Table::GetMeta() const { return meta_; }

string Table::GetName() const { return table_name_; }

bool Table::IsHiddenColumn(const string &col_name) const { return col_name[0] == '-'; }

int Table::GetColumnIdx(string col_name) const {
  // 所有的隐藏列都不满足正常明明要求
  auto col_it = std::find_if(meta_.cols_.begin(), meta_.cols_.end(),
                             [col_name](const Column &col) { return col.name_.compare(col_name) == 0; });
  if (col_it == meta_.cols_.end()) {
    throw ColumnNotExistsError(table_name_, col_name);
  } else {
    return col_it - meta_.cols_.begin();
  }
}

vector<string> Table::GetColumnNames() const {
  // 所有的隐藏列都以"-"为开头
  vector<string> col_names{};
  for (const auto &col : meta_.cols_) {
    if (IsHiddenColumn(col.name_)) continue;
    col_names.push_back(col.name_);
  }
  return col_names;
}

FieldType Table::GetColumnType(int col_idx) const { return meta_.cols_[col_idx].type_; }

}  // namespace dbtrain
