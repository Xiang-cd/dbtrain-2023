#ifndef DBTRAIN_UPDATE_LOG_H
#define DBTRAIN_UPDATE_LOG_H

#include "basic_log.h"
#include "log_image.h"
#include "log_manager.h"

namespace dbtrain {

class UpdateLog : public TxLog {
 public:
  UpdateLog() = default;
  UpdateLog(LSN lsn, LSN prev_lsn, XID xid);
  ~UpdateLog() = default;

  void Load(const Byte *src) override;
  size_t Store(Byte *dst) override;

  virtual void Redo();
  virtual void Undo(LSN lsn);
  UniquePageID GetUniPageID() const;

  LogType GetType() const override;
  size_t GetLength() const override;

 public:
  PhysiologicalImage log_image_;

  friend class LogFactory;
};

class CLRLog: public UpdateLog {
 public:
  CLRLog() = default;
  CLRLog(LSN lsn, LSN prev_lsn, XID xid, LSN undo);
  ~CLRLog() = default;

  void Load(const Byte *src) override;
  size_t Store(Byte *dst) override;


  void from_UpdateLog(const UpdateLog *ulog);
  LogType GetType() const override;
  size_t GetLength() const override;

 public:
  LSN undoNext;

};

}  // namespace dbtrain

#endif
