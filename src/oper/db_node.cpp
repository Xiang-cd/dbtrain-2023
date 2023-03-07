#include "db_node.h"

#include "exception/exceptions.h"
#include "record/fields.h"
#include "record/record_factory.h"

namespace dbtrain {

DeleteNode::DeleteNode(OperNode *child, Table *table) : OperNode({child}), table_(table) {}

RecordList DeleteNode::Next() {
  auto child = childs_[0];
  auto next_list = child->Next();
  // TIPS: Delete Record in each Page
  while (next_list.size() > 0) {
    for (const auto &record : next_list) {
      DeleteRecord(record);
      delete record;
    }
    next_list = child->Next();
  }
  return {};
}

void DeleteNode::DeleteRecord(Record *record) {
  Rid rid = RecordFactory::GetRid(record);
  table_->DeleteRecord(rid);
}

void DeleteNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Delete From (%s):\n", table_->GetName().c_str());
  for (const auto &child : childs_) child->Display(depth + 1);
}

UpdateNode::UpdateNode(OperNode *child, Table *table, const vector<pair<int, Field *>> &set_clause)
    : OperNode({child}), table_(table), set_clause_(set_clause) {}

UpdateNode::~UpdateNode() {
  for (const auto &set_pair : set_clause_) delete set_pair.second;
}

RecordList UpdateNode::Next() {
  auto child = childs_[0];
  auto next_list = child->Next();
  while (next_list.size() > 0) {
    for (const auto &record : next_list) {
      UpdateRecord(record);
      delete record;
    }
    next_list = child->Next();
  }
  return {};
}

void UpdateNode::UpdateRecord(Record *record) {
  Rid rid = RecordFactory::GetRid(record);
  for (const auto &set_pair : set_clause_) {
    record->SetField(set_pair.first, set_pair.second->Copy());
  }
  table_->UpdateRecord(rid, record);
}

void UpdateNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Update (%s):\n", table_->GetName().c_str());
  for (const auto &child : childs_) child->Display(depth + 1);
}

InsertNode::InsertNode(Table *table, const vector<Record *> &records)
    : OperNode({}), table_(table), records_(records) {}

InsertNode::~InsertNode() {
  for (const auto &record : records_) delete record;
}

RecordList InsertNode::Next() {
  // 添加隐藏列
  for (const auto &record : records_) {
    for (int i = 0; i < 4; ++i) {
      record->PushBack(new IntField(0));
    }
  }
  // 执行插入操作
  for (const auto &record : records_) table_->InsertRecord(record);
  return {};
}

void InsertNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Insert (%s):\n", table_->GetName().c_str());
}

SelectNode::SelectNode(OperNode *child) : OperNode({child}) {}

RecordList SelectNode::Next() {
  auto child = childs_[0];
  RecordList record_list{};
  auto next_list = child->Next();
  while (next_list.size() > 0) {
    record_list.insert(record_list.end(), next_list.begin(), next_list.end());
    next_list = child->Next();
  }
  return record_list;
}

void SelectNode::Display(int depth) const {
  for (int i = 0; i < depth; ++i) printf("\t");
  printf("Select:\n");
  for (const auto &child : childs_) child->Display(depth + 1);
}

}  // namespace dbtrain
