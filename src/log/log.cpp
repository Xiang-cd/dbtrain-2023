#include "log.h"

namespace dbtrain {

Log::Log(LSN lsn) : lsn_(lsn) {}

void Log::Load(const Byte *src) { memcpy(&lsn_, src, sizeof(LSN)); }
size_t Log::Store(Byte *dst) {
  memcpy(dst, &lsn_, sizeof(LSN));
  return sizeof(LSN);
}

size_t Log::GetLength() const {
  // TIPS: LogType由LogFactory进行管理
  return sizeof(LSN) + sizeof(LogType);
}

LSN Log::GetLSN() const { return lsn_; }

}  // namespace dbtrain
