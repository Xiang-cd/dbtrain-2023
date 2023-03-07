#ifndef DBTRAIN_INT_RECORD_H
#define DBTRAIN_INT_RECORD_H

#include "field.h"

namespace dbtrain {

class IntField : public Field {
 public:
  IntField(int val);
  IntField();
  ~IntField() = default;
  virtual void Load(const void *src, int s = 0) override;
  virtual void Store(void *dst, int s = 0) const override;
  virtual FieldType GetType() const override;
  virtual bool Equal(Field *field) const override;
  virtual bool Less(Field *field) const override;
  virtual bool Greater(Field *field) const override;
  virtual Field *Copy() const override;
  virtual std::string ToString() const override;
  int GetValue() const;

 protected:
  int val_;
};

}  // namespace dbtrain

#endif
