#ifndef DBTRAIN_FLOAT_RECORD_H
#define DBTRAIN_FLOAT_RECORD_H

#include "field.h"

namespace dbtrain {

class FloatField : public Field {
 public:
  FloatField(double val);
  FloatField();
  ~FloatField() = default;
  virtual void Load(const void *src, int s = 0) override;
  virtual void Store(void *dst, int s = 0) const override;
  virtual FieldType GetType() const override;
  virtual bool Equal(Field *field) const override;
  virtual bool Less(Field *field) const override;
  virtual bool Greater(Field *field) const override;
  virtual Field *Copy() const override;
  virtual std::string ToString() const override;
  double GetValue() const;

 protected:
  double val_;
};

}  // namespace dbtrain

#endif
