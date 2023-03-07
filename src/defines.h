#ifndef DBTRAIN_DEFINES_H
#define DBTRAIN_DEFINES_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace dbtrain {

typedef uint8_t Byte;
typedef int RC;
typedef int FieldSize;
typedef unsigned int PageID;
typedef unsigned int SlotID;
typedef unsigned int LogID;
typedef int PageOffset;
typedef int TableID;
typedef unsigned int LSN;
typedef unsigned int XID;
typedef std::thread::id TID;

static const int PAGE_SIZE = 4096;
static const int BUFFER_SIZE = 50;
static const int MAX_RECORD_SIZE = 2048;
static const int MAX_COLUMN_NUM = 1024;
static const int FD_BITS = 16;

static const int META_PAGE_NO = 0;

static const std::string BASE_PATH = "dbtrain";
static const std::string DB_META_SUFFIX = ".meta";
static const std::string DB_DATA_SUFFIX = ".data";
static const std::string MASTER_RECORD = "MASTER";
static const std::string LOG_INDEX = "LOGIDX";
static const std::string LOG_DATA = "LOGDATA";

static const PageID NULL_PAGE = 0xFFFFFFFF;
static const XID INIT_XID = 0x0001;
static const LSN INIT_LSN = 0x0001;
static const XID INVALID_XID = 0x0;

static const int BITMAP_WIDTH = 8;

static const double EPSILON = 1e-8;

using std::pair;
using std::string;
using std::vector;

enum class FieldType { INT, FLOAT, STRING, DATE, BLOB };

static std::unordered_map<FieldType, std::string> type2str = {{FieldType::INT, "INT"},
                                                              {FieldType::FLOAT, "FLOAT"},
                                                              {FieldType::DATE, "DATE"},
                                                              {FieldType::STRING, "STRING"},
                                                              {FieldType::BLOB, "BLOB"}};

struct Rid {
  PageID page_no;
  SlotID slot_no;
};

}  // namespace dbtrain

#endif  // DBTRAIN_DEFINES_H
