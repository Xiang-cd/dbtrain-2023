#include "table_meta.h"

#include "record/fields.h"

namespace dbtrain {

int TableMeta::Load(const uint8_t *src) {
  // TODO: 元信息反序列化
  // TIPS: 函数目标是通过src反序列化成Meta结构体，通过memcpy函数可以完成这一工作
  // TIPS: 需要注意将所有信息全部保存，变长信息可以通过“长度+数据”的方式存储
  // LAB 1 BEGIN
  // LAB 1 END
}

int TableMeta::Store(uint8_t *dst) {
  // TODO: 元信息序列化
  // TIPS: 函数目标是将Meta信息序列化到dst中，通过memcpy函数可以完成这一工作
  // TIPS: 需要注意将所有信息全部读取，变长信息需要结合Load函数解析
  // LAB 1 BEGIN
  // LAB 1 END
}

size_t TableMeta::GetSize() const { return cols_.size(); }

size_t TableMeta::GetLength() const {
  size_t length = 0;
  for (const auto &col : cols_) {
    length += col.len_;
  }
  return length;
}

PageID TableMeta::GetTableEnd() const { return table_end_page_; }

}  // namespace dbtrain
