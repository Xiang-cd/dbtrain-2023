#include "table_meta.h"

#include "record/fields.h"
#include "utils/debug-print.hpp"
#include <iostream>
#include <vector>
namespace dbtrain {

int TableMeta::Load(const uint8_t *src) {
  // TIPS: 函数目标是通过src反序列化成Meta结构体，通过memcpy函数可以完成这一工作
  // TIPS: 需要注意将所有信息全部保存，变长信息可以通过“长度+数据”的方式存储
  // LAB 1 BEGIN
  vector<Column>().swap(cols_);
  int index = 0;
  unsigned col_num = *(unsigned *) src;
  Print("loaded col_num:", col_num);
  index += sizeof (unsigned );
  for (int i = 0; i < col_num; ++i) {
      Column col = Column();
      memcpy(&col, src+index, 16);
      index += 16;
      unsigned str_len = *(unsigned *) (src + index);
      index += sizeof (unsigned );
//      Print("loaded str len:", str_len);
      char name[str_len];
      memcpy(&name, src + index, str_len);
      col.name_ = string(name);
//      Print("name:", col.name_);
      index += str_len;
      cols_.push_back(col);
  }

  return 0;
  // LAB 1 END
}

int TableMeta::Store(uint8_t *dst) {
  // TIPS: 函数目标是将Meta信息序列化到dst中，通过memcpy函数可以完成这一工作
  // TIPS: 需要注意将所有信息全部读取，变长信息需要结合Load函数解析
  // LAB 1 BEGIN
    int index = 0;
    unsigned col_num = cols_.size();
    memcpy(dst, &col_num, sizeof (unsigned ));
//    Print("stroed col_num:", col_num);
    index += sizeof (unsigned );

    for (auto col: cols_){
        memcpy(dst+index, &col, 16);
        index += 16;
        unsigned str_len = col.name_.size() + 1;
//        Print("str len:", str_len);
        memcpy(dst+index, &str_len, sizeof (unsigned ));
        index += sizeof (unsigned );
//        Print("name:", col.name_);
        strcpy((char*)dst+index, col.name_.c_str());
        index += str_len;
    }
    return  0;
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
