#ifndef DBTRAIN_PRINT_BUFFER_H
#define DBTRAIN_PRINT_BUFFER_H

#include <map>

#include "defines.h"
#include "record/record.h"

namespace dbtrain {

class PrintBuffer {
 public:
  ~PrintBuffer();
  static PrintBuffer &GetInstance();
  PrintBuffer(const PrintBuffer &) = delete;
  void operator=(const PrintBuffer &) = delete;

  void InitBuffer(const std::vector<string> &thread_names);
  void PrintOut(std::string thread_name);
  void PrintRecord(std::string thread_name, const RecordList &records);

 private:
  PrintBuffer() = default;
  std::map<string, char *> buffer_map_;
  std::map<string, int> offset_map_;
};

}  // namespace dbtrain

#endif
