#ifndef DBTRAIN_OPTIMIZOR_H
#define DBTRAIN_OPTIMIZOR_H

#include <unordered_map>

#include "defines.h"
#include "oper/conditions/condition.h"
#include "oper/conditions/join_condition.h"
#include "oper/oper_node.h"
#include "parser/ast.h"
#include "parser/visitor.h"
#include "system/system_manager.h"

namespace dbtrain {

using namespace ast;

class Optimizer : public Visitor {
 public:
  Optimizer();
  ~Optimizer();

 public:
  virtual std::any visit(Select *) override;
  virtual std::any visit(Delete *) override;
  virtual std::any visit(Update *) override;
  virtual std::any visit(Insert *) override;

  virtual std::any visit(EqualConditionNode *) override;
  virtual std::any visit(LessConditionNode *) override;
  virtual std::any visit(GreaterConditionNode *) override;
  virtual std::any visit(JoinConditionNode *) override;
  virtual std::any visit(AndConditionNode *) override;
  virtual std::any visit(OrConditionNode *) override;

  int get_actual_idx(const string & table_name, int col_idx);
  bool is_select;
 private:
  SystemManager *meta_;
  std::unordered_map<string, Condition *> table_filter_{};
  std::unordered_map<string, int> table_shift_{};
  std::unordered_map<string, int> table_record_len{};
  std::unordered_map<string, std::map<int, int>> table_project{};
  std::unordered_map<string, std::map<int, int>> table_project_inv{};
};

}  // namespace dbtrain

#endif
