#include "log_image.h"
#include "utils/debug-print.hpp"
#include "system/system_manager.h"

namespace dbtrain {

PhysiologicalImage::PhysiologicalImage() : old_val_(nullptr), new_val_(nullptr) {}

PhysiologicalImage::~PhysiologicalImage() {
  if (old_val_ != nullptr) delete[] old_val_;
  if (new_val_ != nullptr) delete[] new_val_;
}

void PhysiologicalImage::Load(const Byte *src) {
  // TODO: Log Image反序列化
  // TIPS: 根据操作类型区分
  // LAB 2 BEGIN
  size_t offset = 0;
  memcpy(&op_type_, src+offset, sizeof (LogOpType)); offset += sizeof(LogOpType);
  size_t name_len = *(size_t *) (src + offset); offset += sizeof(size_t);
  char array[name_len + 1];
  memcpy(array,src + offset, name_len); array[name_len] = '\0';
  table_name_ = std::string (array);
  offset += name_len;

  memcpy(&page_id_, src + offset, sizeof(PageID)); offset += sizeof(PageID);
  memcpy(&slot_id_, src + offset, sizeof(SlotID)); offset += sizeof(SlotID);
  if (op_type_ ==  LogOpType::INSERT){
    memcpy( &new_len_,src + offset,  sizeof(size_t)); offset += sizeof(size_t);
    new_val_ = new Byte [new_len_];
    memcpy( new_val_, src + offset, new_len_); offset += new_len_;
  }else if (op_type_ == LogOpType::DELETE){
    memcpy( &old_len_,src + offset,  sizeof(size_t)); offset += sizeof(size_t);
    old_val_ = new Byte [old_len_];
    memcpy( old_val_, src + offset, old_len_); offset += old_len_;
  }else if (op_type_ == LogOpType::UPDATE){
    memcpy( &new_len_,src + offset,  sizeof(size_t)); offset += sizeof(size_t);
    memcpy( &old_len_,src + offset,  sizeof(size_t)); offset += sizeof(size_t);
    new_val_ = new Byte [new_len_];
    old_val_ = new Byte [old_len_];
    memcpy( new_val_, src + offset, new_len_); offset += new_len_;
    memcpy( old_val_, src + offset, old_len_); offset += old_len_;
  }else{
    assert(false);
  }
  // LAB 2 END
}

size_t PhysiologicalImage::Store(Byte *dst) {
  // TODO: Log Image序列化
  // TIPS: 根据操作类型区分，返回Store的数据长度
  // LAB 2 BEGIN
  size_t offset = 0;
  memcpy(dst+offset, &op_type_, sizeof (LogOpType)); offset += sizeof(LogOpType);
  *(size_t *) (dst + offset) = table_name_.size(); offset += sizeof(size_t);
  memcpy(dst+offset, table_name_.c_str(), table_name_.size()); offset += table_name_.size();
  memcpy(dst + offset, &page_id_, sizeof(PageID)); offset += sizeof(PageID);
  memcpy(dst + offset, &slot_id_, sizeof(SlotID)); offset += sizeof(SlotID);

  if (op_type_ ==  LogOpType::INSERT){
    memcpy(dst + offset, &new_len_, sizeof(size_t)); offset += sizeof(size_t);
    memcpy(dst + offset, new_val_, new_len_); offset += new_len_;
  }else if (op_type_ == LogOpType::DELETE){
    memcpy(dst + offset, &old_len_, sizeof(size_t)); offset += sizeof(size_t);
    memcpy(dst + offset, old_val_, old_len_); offset += old_len_;
  }else if (op_type_ == LogOpType::UPDATE){
    memcpy(dst + offset, &new_len_, sizeof(size_t)); offset += sizeof(size_t);
    memcpy(dst + offset, &old_len_, sizeof(size_t)); offset += sizeof(size_t);
    memcpy(dst + offset, new_val_, new_len_); offset += new_len_;
    memcpy(dst + offset, old_val_, old_len_); offset += old_len_;
  }else{
    assert(false);
  }

  return offset;
  // LAB 2 END
}

size_t PhysiologicalImage::GetLength() const {
  // TODO: 获取Log Image的长度
  // TIPS: 根据操作类型区分
  // LAB 2 BEGIN
  size_t base_len = sizeof(LogOpType) +sizeof(size_t) +table_name_.size() +sizeof(PageID) +  sizeof(SlotID);
  size_t extra_len = 0;
  if (op_type_ ==  LogOpType::INSERT){
    extra_len += sizeof(size_t) + new_len_;
  }else if (op_type_ == LogOpType::DELETE){
    extra_len += sizeof(size_t) + old_len_;
  }else if (op_type_ == LogOpType::UPDATE){
    extra_len += sizeof(size_t) * 2 + old_len_ + new_len_;
  }

  return extra_len + base_len;
  // LAB 2 END
}

}  // namespace dbtrain
