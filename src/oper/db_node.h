#ifndef DBTRAIN_DB_NODE_H
#define DBTRAIN_DB_NODE_H

#include "oper_node.h"
#include "table/table.h"

namespace dbtrain {

class DeleteNode : public OperNode {
 public:
  DeleteNode(OperNode *child, Table *table);
  ~DeleteNode() = default;
  RecordList Next() override;

  virtual void Display(int depth) const override;

 private:
  void DeleteRecord(Record *);
  Table *table_;
};

class UpdateNode : public OperNode {
 public:
  UpdateNode(OperNode *child, Table *table, const vector<pair<int, Field *>> &set_clause);
  ~UpdateNode();
  RecordList Next() override;

  virtual void Display(int depth) const override;

 private:
  Table *table_;
  vector<pair<int, Field *>> set_clause_;
  void UpdateRecord(Record *);
};

class InsertNode : public OperNode {
 public:
  InsertNode(Table *table, const vector<Record *> &records);
  ~InsertNode();
  RecordList Next() override;

  virtual void Display(int depth) const override;

 private:
  Table *table_;
  vector<Record *> records_;
};

class SelectNode : public OperNode {
 public:
  SelectNode(OperNode *child);
  ~SelectNode() = default;
  RecordList Next() override;

  virtual void Display(int depth) const override;

 private:
};

}  // namespace dbtrain

#endif
