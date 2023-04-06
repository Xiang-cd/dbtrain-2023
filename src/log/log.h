#ifndef DBTRAIN_LOG_H
#define DBTRAIN_LOG_H

#include "defines.h"

namespace dbtrain {

const LSN NULL_LSN = UINT32_MAX;

enum class LogType { UNDEFINED = 0, UPDATE, COMMIT, ABORT, BEGIN, END, CLR, CHECKPOINT };
static std::unordered_map<LogType, std::string> LogType2str = {
    {LogType::UNDEFINED, "UNDEFINED"},
    {LogType::UPDATE, "UPDATE"},
    {LogType::COMMIT, "COMMIT"},
    {LogType::ABORT, "ABORT"},
    {LogType::BEGIN, "BEGIN"},
    {LogType::END, "END"},
    {LogType::CLR, "CLR"},
    {LogType::CHECKPOINT, "CHECKPOINT"},
};

class Log {
 public:
  Log() = default;
  Log(LSN lsn);
  virtual ~Log() = default;

  virtual void Load(const Byte *src);
  virtual size_t Store(Byte *dst);

  virtual LogType GetType() const = 0;
  virtual size_t GetLength() const;
  LSN GetLSN() const;

 protected:
  LSN lsn_;
};

}  // namespace dbtrain

#endif