#ifndef DBTRAIN_varchar_RECORD_H
#define DBTRAIN_varchar_RECORD_H

#include "field.h"
#include "str_field.h"

namespace dbtrain {

class VarCharField : public Field {
 public:
  VarCharField(const char *src, int size);
  VarCharField(int size);
  VarCharField(const StrField *f);
  ~VarCharField();
  virtual void Load(const void *src, int s = 0) override;
  virtual void Store(void *dst, int s = 0) const override;
  virtual FieldType GetType() const override;
  virtual bool Equal(Field *field) const override;
  virtual bool Less(Field *field) const override;
  virtual bool Greater(Field *field) const override;
  virtual Field *Copy() const override;
  virtual std::string ToString() const override;
  string GetValue() const;

 protected:
  char *val_;
};

}  // namespace dbtrain

#endif
