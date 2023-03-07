#include "log_image.h"

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
  // LAB 2 END
}

size_t PhysiologicalImage::Store(Byte *dst) {
  // TODO: Log Image序列化
  // TIPS: 根据操作类型区分，返回Store的数据长度
  // LAB 2 BEGIN
  // LAB 2 END
}

size_t PhysiologicalImage::GetLength() const {
  // TODO: 获取Log Image的长度
  // TIPS: 根据操作类型区分
  // LAB 2 BEGIN
  // LAB 2 END
}

}  // namespace dbtrain
