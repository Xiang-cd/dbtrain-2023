#ifndef DBTRAIN_DBEXCEPTION_H
#define DBTRAIN_DBEXCEPTION_H

#include <cstring>
#include <stdexcept>

namespace dbtrain {

class DbError : public std::runtime_error {
 public:
  explicit DbError(const std::string &message) : std::runtime_error("Error: " + message) {}
  ~DbError() = default;
};

class FileExistsError : public DbError {
 public:
  FileExistsError(const std::string &filename) : DbError("File Exists: " + filename) {}
};

class FileNotExistsError : public DbError {
 public:
  FileNotExistsError(const std::string &filename) : DbError("File Not Exists: " + filename) {}
};

class FileNotClosedError : public DbError {
 public:
  FileNotClosedError(const std::string &filename) : DbError("File Not Closed: " + filename) {}
};

class FileNotOpenError : public DbError {
 public:
  FileNotOpenError(int fd) : DbError("File Descriptor Not Open: " + std::to_string(fd)) {}
};

class UnknownError : public DbError {
 public:
  UnknownError() : DbError("Unknown Error") { perror("Error"); }
};

class DropUsingDatabaseError : public DbError {
 public:
  DropUsingDatabaseError() : DbError("Cannot drop the currently open database") {}
};

class DatabaseNotExistsError : public DbError {
 public:
  DatabaseNotExistsError(std::string db_name) : DbError("Database '" + db_name + "' doesn't exist") {}
};

class DatabaseExistsError : public DbError {
 public:
  DatabaseExistsError(std::string db_name) : DbError("Database '" + db_name + "' already exists") {}
};

class NoUsingDatabaseError : public DbError {
 public:
  NoUsingDatabaseError() : DbError("No database selected") {}
};

class InvalidRecordSizeError : public DbError {
 public:
  InvalidRecordSizeError(int record_length) : DbError("Invalid column length" + std::to_string(record_length)) {}
};

class TableExistsError : public DbError {
 public:
  TableExistsError(std::string table_name) : DbError("Table '" + table_name + "' already exists") {}
};

class TableNotExistsError : public DbError {
 public:
  TableNotExistsError(std::string table_name) : DbError("Table '" + table_name + "' doesn't exist") {}
};

class InvalidInsertCountError : public DbError {
 public:
  InvalidInsertCountError(size_t insert_size, size_t col_size)
      : DbError("Invalid insert count: inserted " + std::to_string(insert_size) +
                ", column count: " + std::to_string(col_size)) {}
};

class InvalidInsertTypeError : public DbError {
 public:
  InvalidInsertTypeError(std::string insert_type, std::string col_type)
      : DbError("Invalid insert type: inserted " + insert_type + ", column type: " + col_type) {}
};

class InvalidUpdateTypeError : public DbError {
 public:
  InvalidUpdateTypeError(std::string update_type, std::string col_type)
      : DbError("Invalid update type: updated " + update_type + ", column type: " + col_type) {}
};

class InvalidInsertStringLenError : public DbError {
 public:
  InvalidInsertStringLenError(size_t insert_len, size_t col_len)
      : DbError("Invalid insert length: inserted " + std::to_string(insert_len) +
                ", column length: " + std::to_string(col_len)) {}
};

class DuplicateColumnError : public DbError {
 public:
  DuplicateColumnError(std::string col_name) : DbError("Duplicate column " + col_name) {}
};

class ColumnNotFoundError : public DbError {
 public:
  ColumnNotFoundError(std::string col_name) : DbError("Column " + col_name + " not found in field list") {}
};

class ColumnAmbiguousError : public DbError {
 public:
  ColumnAmbiguousError(std::string col_name) : DbError("Column " + col_name + " in field list is ambiguous") {}
};

}  // namespace dbtrain

#endif  // DBTRAIN_DBEXCEPTION_H
