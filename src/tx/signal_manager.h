#ifndef DBTRAIN_SIGNAL_MANAGER_H
#define DBTRAIN_SIGNAL_MANAGER_H

#include <mutex>
#include <set>

#include "defines.h"

namespace dbtrain {

class SignalManager {
 public:
  ~SignalManager() = default;
  static SignalManager &GetInstance();
  SignalManager(const SignalManager &) = delete;
  void operator=(const SignalManager &) = delete;

  void CreateSignal(string sname);
  void WaitSignal(string sname);

 private:
  SignalManager() = default;
  std::mutex set_mutex_;
  std::set<string> signal_set_;
};

}  // namespace dbtrain

#endif