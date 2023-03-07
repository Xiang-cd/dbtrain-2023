#include "result.h"

namespace dbtrain {

Result::Result(const std::vector<std::string> header, const RecordList &records) : header_(header), records_(records) {}

Result::Result(const std::vector<std::string> header) : header_(header) {}

Result::Result(const RecordList &records) : records_(records) {}

Result::~Result() {
  header_.clear();
  records_.clear();
}

void Result::InsertRecords(const RecordList &records) {
  records_.insert(std::end(records_), records.begin(), records.end());
}

}  // namespace dbtrain
