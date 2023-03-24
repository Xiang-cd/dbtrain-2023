#ifndef DBTRAIN_RECORD_FACTORY_H
#define DBTRAIN_RECORD_FACTORY_H

#include "defines.h"
#include "record.h"
#include "table/table_meta.h"

namespace dbtrain {

class RecordFactory {
 public:
  static Record *ConstInt(int v);
  static Record *ConstFloat(double v);
  static Record *ConstString(string v);

  RecordFactory(const TableMeta *meta);
  ~RecordFactory();

  Record *LoadRecord(const uint8_t *src) const;
  void StoreRecord(uint8_t *dst, Record *record) const;

  static Field *LoadField(const uint8_t *src, FieldType ft, FieldSize fs);
  static void StoreField(uint8_t *dst, Field *field, FieldType ft, FieldSize fs);
  int GetStoreSize(Record * r);

  // TIPS: 隐藏列操作
  static Rid GetRid(Record *record);
  static void SetRid(Record *record, Rid rid);

  // TODO: 设置MVCC的相关隐藏列
  // LAB 3 BEGIN
  // LAB 3 END

 private:
  const TableMeta *meta_;
};

typedef RecordFactory TupleFactory;

}  // namespace dbtrain

#endif
