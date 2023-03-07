#include "signal_manager.h"

#include <unistd.h>

namespace dbtrain {

const int LOCK_DELAY = 100;
const int WAIT_DELAY = 1000;

SignalManager &SignalManager::GetInstance() {
  static SignalManager signal_manager;
  return signal_manager;
}

void SignalManager::CreateSignal(string sname) {
  set_mutex_.lock();
  assert(signal_set_.find(sname) == signal_set_.end());
  signal_set_.insert(sname);
  set_mutex_.unlock();
}

void SignalManager::WaitSignal(string sname) {
  do {
    while (set_mutex_.try_lock() == false) {
      usleep(LOCK_DELAY);
    }
    if (signal_set_.find(sname) != signal_set_.end()) break;
    set_mutex_.unlock();
    usleep(WAIT_DELAY);
  } while (true);
  set_mutex_.unlock();
}

}  // namespace dbtrain
