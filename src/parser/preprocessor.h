#ifndef DBTRAIN_TABLENAME_VISITOR_H
#define DBTRAIN_TABLENAME_VISITOR_H

#include "parser/ast.h"
#include "parser/visitor.h"
#include "system/system_manager.h"

namespace dbtrain {

namespace ast {

/**
 * @brief The TableNameVisitor class
 *
 * 添加表名
 */
class Preprocessor : public Visitor {
 public:
  Preprocessor(std::vector<std::string> table_names);
  ~Preprocessor() = default;

  virtual std::any visit(Select *) override;
  virtual std::any visit(Update *) override;
  virtual std::any visit(Delete *) override;

  virtual std::any visit(EqualConditionNode *) override;
  virtual std::any visit(LessConditionNode *) override;
  virtual std::any visit(GreaterConditionNode *) override;
  virtual std::any visit(JoinConditionNode *) override;
  virtual std::any visit(AndConditionNode *) override;
  virtual std::any visit(OrConditionNode *) override;

 private:
  std::vector<std::string> table_names_;
  SystemManager &meta_;
};

}  // namespace ast

}  // namespace dbtrain

#endif  // DBTRAIN_TABLENAME_VISITOR_H
