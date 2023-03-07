#include "table_printer.h"

#include <iomanip>
#include <iostream>

namespace dbtrain {

void TablePrinter::Print(const Result *result) {
  GetWidths(result);
  PrintSeparator();
  PrintHeader(result);
  PrintSeparator();
  if (!result->records_.empty()) {
    PrintRecord(result);
    PrintSeparator();
    PrintCount(result);
  }
  std::cout << "\n";
  Clear(result);
}

void TablePrinter::GetWidths(const Result *result) {
  for (size_t i = 0; i < result->header_.size(); i++) {
    widths_.push_back(result->header_[i].size());
  }
  for (const auto &record : result->records_) {
    for (size_t i = 0; i < record->GetSize(); i++) {
      widths_[i] =
          record->GetField(i)->ToString().size() > widths_[i] ? record->GetField(i)->ToString().size() : widths_[i];
    }
  }
}

void TablePrinter::PrintSeparator() {
  for (auto width : widths_) {
    std::cout << '+' << std::string(width + 2, '-');
  }
  std::cout << "+\n";
}

void TablePrinter::PrintHeader(const Result *result) {
  for (size_t i = 0; i < result->header_.size(); i++) {
    std::cout << "| " << std::left << std::setw(widths_[i]) << result->header_[i] << ' ';
  }
  std::cout << "|\n";
}

void TablePrinter::PrintRecord(const Result *result) {
  for (const auto &record : result->records_) {
    for (size_t i = 0; i < record->GetSize(); i++) {
      std::cout << "| " << std::left << std::setw(widths_[i]) << record->GetField(i)->ToString() << ' ';
    }
    std::cout << "|\n";
  }
}

void TablePrinter::PrintCount(const Result *result) { std::cout << "(" << result->records_.size() << " rows)\n"; }

void TablePrinter::Clear(const Result *result) {
  widths_.clear();
  for (auto &record : result->records_) {
    delete record;
  }
}

}  // namespace dbtrain
