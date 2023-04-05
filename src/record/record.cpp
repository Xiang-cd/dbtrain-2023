#include "record.h"

#include <iostream>

namespace dbtrain {

Record::~Record() {
  for (const auto &field : field_list_) delete field;
}

Field *Record::GetField(int idx) const {
  assert(idx < field_list_.size());
  return field_list_[idx];
}

void Record::SetField(int idx, Field *field) {
  assert(idx < field_list_.size());
  delete field_list_[idx];
  field_list_[idx] = field;
}

void Record::PushBack(Field *field) { field_list_.push_back(field); }

Record *Record::Copy() const {
  Record *record = new Record();
  for (const auto &field : field_list_) record->field_list_.push_back(field->Copy());
  return record;
}

size_t Record::GetNumField() const { return field_list_.size(); }

void Record::Display() const {
  for (Field *field : field_list_) {
    std::cout << field->ToString() << " ";
  }
  std::cout << std::endl;
}

std::vector<std::string> Record::ToString() const {
  std::vector<std::string> s;
  for (Field *field : field_list_) {
    s.push_back(field->ToString());
  }
  return s;
}

}  // namespace dbtrain
