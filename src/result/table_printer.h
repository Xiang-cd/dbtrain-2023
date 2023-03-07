#ifndef DBTRAIN_TABLE_PRINTER_H
#define DBTRAIN_TABLE_PRINTER_H

#include "printer.h"

namespace dbtrain {

class TablePrinter : public Printer {
 public:
  void Print(const Result *result);

 private:
  void GetWidths(const Result *result);
  void PrintSeparator();
  void PrintHeader(const Result *result);
  void PrintRecord(const Result *result);
  void PrintCount(const Result *result);
  void Clear(const Result *result);

  std::vector<size_t> widths_;
};

}  // namespace dbtrain

#endif  // DBTRAIN_TABLE_PRINTER_H
