#ifndef DBTRAIN_RAW_PRINTER_H
#define DBTRAIN_RAW_PRINTER_H

#include "printer.h"

namespace dbtrain {

class RawPrinter : public Printer {
 public:
  void Print(const Result *result);
};

}  // namespace dbtrain

#endif  // DBTRAIN_RAW_PRINTER_H
