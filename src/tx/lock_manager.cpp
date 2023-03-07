#include "lock_manager.h"

#include <ctime>

namespace dbtrain {

LockManager &LockManager::GetInstance() {
  static LockManager lock_manager;
  return lock_manager;
}

LockManager::LockManager() {}

LockManager::~LockManager() {
  for (auto &pair : lock_map_) {
    delete pair.second;
  }
}

std::shared_mutex *LockManager::GetLock(string lname) {
  map_lock_.lock();
  if (lock_map_.find(lname) == lock_map_.end()) {
    lock_map_[lname] = new std::shared_mutex();
  }
  std::shared_mutex *mtx = lock_map_[lname];
  map_lock_.unlock();
  return mtx;
}

void LockManager::Lock(string lname) {
  std::shared_mutex *mtx = GetLock(lname);
  mtx->lock();
}

void LockManager::Unlock(string lname) {
  std::shared_mutex *mtx = GetLock(lname);
  mtx->unlock();
}

void LockManager::LockShared(string lname) {
  std::shared_mutex *mtx = GetLock(lname);
  mtx->lock_shared();
}

void LockManager::UnlockShared(string lname) {
  std::shared_mutex *mtx = GetLock(lname);
  mtx->unlock_shared();
}

}  // namespace dbtrain
