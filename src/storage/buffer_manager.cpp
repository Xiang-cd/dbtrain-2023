#include "buffer_manager.h"

#include <cassert>
#include "utils/debug-print.hpp"
namespace dbtrain {

BufferManager::BufferManager() : disk_manager_(DiskManager::GetInstance()), log_manager_(LogManager::GetInstance()) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    free_list_.push_back(i);
  }
}

BufferManager::~BufferManager() { FlushAll(); }

BufferManager &BufferManager::GetInstance() {
  static BufferManager buffer_manager;
  return buffer_manager;
}

Page *BufferManager::AllocPage(int fd, PageID page_no) {
  Page *page = FetchPage(fd, page_no);
  return page;
}

Page *BufferManager::GetPage(int fd, PageID page_no) {
  Page *page;
//  Print("buffer manager getting page");
  FilePageId page_id = {fd, page_no};
  auto iter_page = hashmap_.find(page_id);
  if (iter_page != hashmap_.end()) {
    int frame_no = iter_page->second;
    Access(frame_no);
    page = &frames_[frame_no];
  } else {
    page = FetchPage(fd, page_no);
    disk_manager_.ReadPage(fd, page_no, page->data_);
  }
  return page;
}

void BufferManager::Clear() {
  free_list_.insert(free_list_.end(), busy_list_.begin(), busy_list_.end());
  busy_list_.clear();
  hashmap_.clear();
}

void BufferManager::FlushFile(int fd) {
  auto iter = hashmap_.begin();
  while (iter != hashmap_.end()) {
    auto prev = iter;
    iter++;
    if (prev->first.fd == fd) {
      FlushPage(&frames_[prev->second]);
    }
  }
}

void BufferManager::FlushAll() {
  for (auto frame_no : busy_list_) {
    WriteBack(&frames_[frame_no]);
  }
  free_list_.insert(free_list_.end(), busy_list_.begin(), busy_list_.end());
  busy_list_.clear();
  hashmap_.clear();
}

bool BufferManager::PageInCache(const FilePageId file_page_id) { return hashmap_.count(file_page_id) > 0; }

void BufferManager::FlushPage(Page *page) {
//  Print("buffer namager flush page");
  assert(PageInCache(page->page_id_));
  WriteBack(page);
  int frame_no = hashmap_[page->page_id_];
  busy_list_.remove(frame_no);
  free_list_.push_front(frame_no);
  hashmap_.erase(page->page_id_);
  assert(!PageInCache(page->page_id_));
}

Page *BufferManager::FetchPage(int fd, PageID page_no) {
  Page *page;
  int frame_no = -1;
  FilePageId page_id = {fd, page_no};
  auto iter_page = hashmap_.find(page_id);
  assert(iter_page == hashmap_.end());  // Page not in memory
  if (free_list_.empty()) {
    // Buffer full
    frame_no = busy_list_.back();
    busy_list_.pop_back();
    WriteBack(&frames_[frame_no]);
    hashmap_.erase(frames_[frame_no].page_id_);
  } else {
    // Buffer not full
    frame_no = free_list_.front();
    free_list_.pop_front();
  }
  page = &frames_[frame_no];
  page->page_id_ = page_id;
  page->is_dirty_ = false;
  busy_list_.push_front(frame_no);
  hashmap_[page_id] = frame_no;
  return page;
}

void BufferManager::Access(int frame_no) {
  busy_list_.remove(frame_no);
  busy_list_.push_front(frame_no);
}

void BufferManager::WriteBack(Page *page) {
  if (page->is_dirty_) {
    log_manager_.WritePage(page->page_id_.fd, page->page_id_.page_no);
    disk_manager_.WritePage(page->page_id_.fd, page->page_id_.page_no, page->data_);
    page->is_dirty_ = false;
  }
}

}  // namespace dbtrain
