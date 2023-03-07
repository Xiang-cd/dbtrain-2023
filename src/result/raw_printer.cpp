#include "raw_printer.h"

#include <iostream>

namespace dbtrain {

void RawPrinter::Print(const Result *result) {
  for (size_t i = 0; i < result->header_.size(); i++) {
    std::cout << result->header_[i];
    if (i != result->header_.size() - 1) {
      std::cout << " | ";
    }
  }
  std::cout << "\n";
  for (const auto &record : result->records_) {
    for (size_t i = 0; i < record->GetSize(); i++) {
      std::cout << record->GetField(i)->ToString();
      if (i != record->GetSize() - 1) {
        std::cout << " | ";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

}  // namespace dbtrain
