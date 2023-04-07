#ifndef DBTRAIN_CHECKPOINT_LOG_H
#define DBTRAIN_CHECKPOINT_LOG_H

#include "defines.h"
#include "log.h"
#include "log_manager.h"

namespace dbtrain {

class CheckpointLog : public Log {
 public:
  CheckpointLog();
  ~CheckpointLog() = default;
  CheckpointLog(LSN lsn);
  void Load(const Byte *src) override;
  size_t Store(Byte *dst) override;
  LogType GetType() const override;
  size_t GetLength() const override;
};


class CkptEndLog : public Log {
 public:
  CkptEndLog();
  ~CkptEndLog() = default;
  CkptEndLog(LSN lsn);
  LogType GetType() const override;
  size_t GetLength() const override;
};

}  // namespace dbtrain

#endif