//
// Created by 项小羽 on 2023/3/17.
//
#include "var_char_field.h"
#include "utils/debug-print.hpp"
#include "exception/record_exceptions.h"

namespace dbtrain {

VarCharField::VarCharField(const char *src, int size) {
  Print("var char constructed");
  size_ = size;
  val_ = new char[size_ + 1];
  val_[size_] = '\0';
  memcpy(val_, src, size_);
}

VarCharField::VarCharField(const StrField *f) {
  Print("var char constructed");
  size_ = f->GetSize();
  val_ = f->val_;
}

VarCharField::VarCharField(int size) {
  Print("var char constructed");
  size_ = size;
  val_ = new char[size_ + 1];
  val_[size_] = '\0';
  memset(val_, 0, size_);
}

VarCharField::~VarCharField() { delete[] val_; }

void VarCharField::Load(const void *src, int s) { memcpy(val_, src, s); }

void VarCharField::Store(void *dst, int s) const {
  if (s <= size_ + 1) {
    memcpy(dst, val_, s);
  } else {
    memcpy(dst, val_, size_ + 1);
  }
}
FieldType VarCharField::GetType() const { return FieldType::VCHAR; }

Field *VarCharField::Copy() const { return new VarCharField(val_, size_); }

bool VarCharField::Equal(Field *field) const {
  if (field->GetType() == FieldType::VCHAR) {
    auto val = dynamic_cast<VarCharField *>(field)->GetValue();
    bool res = string(val_) == val;
    return res;
  } else {
    throw UnsupportOpError();
  }
}
bool VarCharField::Less(Field *field) const {
  if (field->GetType() == FieldType::VCHAR) {
    auto val = dynamic_cast<VarCharField *>(field)->GetValue();
    bool res = string(val_) < val;
    return res;
  } else {
    throw UnsupportOpError();
  }
}
bool VarCharField::Greater(Field *field) const {
  if (field->GetType() == FieldType::VCHAR) {
    auto val = dynamic_cast<VarCharField *>(field)->GetValue();
    bool res = string(val_) > val;
    return res;
  } else {
    throw UnsupportOpError();
  }
}

std::string VarCharField::ToString() const { return val_; }

string VarCharField::GetValue() const { return val_; }

}  // namespace dbtrain
