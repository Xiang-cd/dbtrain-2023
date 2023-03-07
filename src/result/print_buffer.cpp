#include "print_buffer.h"

namespace dbtrain {

const int PRINT_BUFFER_SIZE = 4096;

PrintBuffer &PrintBuffer::GetInstance() {
  static PrintBuffer print_buffer;
  return print_buffer;
}

PrintBuffer::~PrintBuffer() {
  for (const auto &pair : buffer_map_) {
    if (pair.second) delete[] pair.second;
  }
}

void PrintBuffer::InitBuffer(const std::vector<string> &thread_names) {
  for (const auto &pair : buffer_map_) {
    if (pair.second) delete[] pair.second;
  }
  buffer_map_.clear();
  for (const auto &thread_name : thread_names) {
    char *buffer = new char[PRINT_BUFFER_SIZE];
    buffer_map_[thread_name] = buffer;
    offset_map_[thread_name] = 0;
  }
}

void PrintBuffer::PrintOut(string thread_name) {
  printf("%s\n", thread_name.c_str());
  // Remove last '\n'
  buffer_map_[thread_name][offset_map_[thread_name] - 1] = '\0';
  printf("%s\n", buffer_map_[thread_name]);
}

void PrintBuffer::PrintRecord(string thread_name, const RecordList &records) {
  if (!records.empty()) {
    char *buffer = buffer_map_[thread_name];
    for (const auto *record : records) {
      for (size_t i = 0; i < record->GetSize(); i++) {
        snprintf(buffer + offset_map_[thread_name], PRINT_BUFFER_SIZE - offset_map_[thread_name], "%s",
                 record->GetField(i)->ToString().c_str());
        offset_map_[thread_name] += record->GetField(i)->ToString().size();
        if (i != record->GetSize() - 1) {
          snprintf(buffer + offset_map_[thread_name], PRINT_BUFFER_SIZE - offset_map_[thread_name], " | ");
          offset_map_[thread_name] += 3;
        }
      }
      snprintf(buffer + offset_map_[thread_name], PRINT_BUFFER_SIZE - offset_map_[thread_name], "\n");
      offset_map_[thread_name] += 1;
    }
    snprintf(buffer + offset_map_[thread_name], PRINT_BUFFER_SIZE - offset_map_[thread_name], "\n");
    offset_map_[thread_name] += 1;
  }
}

}  // namespace dbtrain
