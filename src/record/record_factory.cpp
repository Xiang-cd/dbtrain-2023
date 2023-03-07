#include "record_factory.h"

#include "exception/exceptions.h"
#include "exception/record_exceptions.h"
#include "fields.h"
#include "table/hidden.h"

namespace dbtrain {

RecordFactory::RecordFactory(const TableMeta *meta) : meta_(meta) {}

RecordFactory::~RecordFactory() {}

Record *RecordFactory::ConstInt(int v) {
  auto record = new Record();
  record->field_list_.push_back(new IntField(v));
  return record;
}

Record *RecordFactory::ConstFloat(double v) {
  auto record = new Record();
  record->field_list_.push_back(new FloatField(v));
  return record;
}

Record *RecordFactory::ConstString(string v) {
  auto record = new Record();
  record->field_list_.push_back(new StrField(v.c_str(), v.size()));
  return record;
}

Field *RecordFactory::LoadField(const void *src, FieldType ft, FieldSize fs) {
  // TODO: 字段反序列化
  // TIPS: 利用Field的Load函数，分类返回对应类型的Field指针
  // LAB 1 BEGIN
  // LAB 1 END
}

void RecordFactory::StoreField(void *dst, Field *field, FieldType ft, FieldSize fs) {
  // TODO: 字段序列化
  // TIPS: 利用Field的Store函数
  // LAB 1 BEGIN
  // LAB 1 END
}

Record *RecordFactory::LoadRecord(const void *src) const {
  // TODO: 记录反序列化
  // TIPS: 通过TableMeta可以读取各个字段的属性和长度，利用LoadField函数建立各个字段对应的Field指针。
  // LAB 1 BEGIN
  // LAB 1 END
}

void RecordFactory::StoreRecord(void *dst, Record *record) const {
  // TODO: 记录序列化
  // TIPS: 通过TableMeta可以读取各个字段的属性和长度，利用StoreField函数对于各个指针进行序列化。
  // TIPS: 友元可以直接访问私有变量
  // LAB 1 BEGIN
  // LAB 1 END
}

Rid RecordFactory::GetRid(Record *record) {
  // 读取隐藏列的RID信息
  PageID page_id = dynamic_cast<IntField *>(record->field_list_[record->GetSize() - PAGE_ID_OFFSET])->GetValue();
  SlotID slot_id = dynamic_cast<IntField *>(record->field_list_[record->GetSize() - SLOT_ID_OFFSET])->GetValue();
  return {page_id, slot_id};
}

void RecordFactory::SetRid(Record *record, Rid rid) {
  // 设置隐藏列的RID信息
  Field *page_field = new IntField(rid.page_no);
  Field *slot_field = new IntField(rid.slot_no);
  record->SetField(record->GetSize() - PAGE_ID_OFFSET, page_field);
  record->SetField(record->GetSize() - SLOT_ID_OFFSET, slot_field);
}

// TODO: 设置MVCC相关隐藏列的接口
// TIPS: 基础功能仅需要设置创建版本号和删除版本号
// LAB 3 BEGIN
// LAB 3 END

}  // namespace dbtrain
