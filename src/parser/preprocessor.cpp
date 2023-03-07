#include "preprocessor.h"

namespace dbtrain {

namespace ast {

Preprocessor::Preprocessor(std::vector<std::string> table_names)
    : table_names_(table_names), meta_(SystemManager::GetInstance()) {}

std::any Preprocessor::visit(Select *select) {
  // 填充缺省表名
  for (const auto &col : select->cols_) {
    if (col->table_name_.empty()) {
      col->table_name_ = meta_.GetTableByColumn(col->col_name_, table_names_);
    }
  }
  if (select->condition_ != nullptr) {
    select->condition_->accept(this);
  }

  // 解析select *
  if (select->cols_.empty()) {
    for (const auto &table_name : select->tables_) {
      auto col_names = SystemManager::GetInstance().GetTable(table_name)->GetColumnNames();
      for (const auto &col_name : col_names) {
        std::shared_ptr<Col> pCol(new Col(table_name, col_name));
        select->cols_.push_back(pCol);
      }
    }
  }
  return nullptr;
}

std::any Preprocessor::visit(Delete *delete_) {
  if (delete_->condition_ != nullptr) {
    delete_->condition_->accept(this);
  }
  return nullptr;
}

std::any Preprocessor::visit(Update *update) {
  if (update->condition_ != nullptr) {
    update->condition_->accept(this);
  }
  return nullptr;
}

std::any Preprocessor::visit(EqualConditionNode *equal_cond) {
  if (equal_cond->col_->table_name_.empty()) {
    equal_cond->col_->table_name_ = meta_.GetTableByColumn(equal_cond->col_->col_name_, table_names_);
  }
  return nullptr;
}

std::any Preprocessor::visit(LessConditionNode *less_cond) {
  if (less_cond->col_->table_name_.empty()) {
    less_cond->col_->table_name_ = meta_.GetTableByColumn(less_cond->col_->col_name_, table_names_);
  }
  return nullptr;
}

std::any Preprocessor::visit(GreaterConditionNode *greater_cond) {
  if (greater_cond->col_->table_name_.empty()) {
    greater_cond->col_->table_name_ = meta_.GetTableByColumn(greater_cond->col_->col_name_, table_names_);
  }
  return nullptr;
}

std::any Preprocessor::visit(JoinConditionNode *join_cond) {
  if (join_cond->lhs_->table_name_.empty()) {
    join_cond->lhs_->table_name_ = meta_.GetTableByColumn(join_cond->lhs_->col_name_, table_names_);
  }
  if (join_cond->rhs_->table_name_.empty()) {
    join_cond->rhs_->table_name_ = meta_.GetTableByColumn(join_cond->rhs_->col_name_, table_names_);
  }
  return nullptr;
}

std::any Preprocessor::visit(AndConditionNode *and_cond) {
  and_cond->lhs_->accept(this);
  and_cond->rhs_->accept(this);
  return nullptr;
}

std::any Preprocessor::visit(OrConditionNode *or_cond) {
  or_cond->lhs_->accept(this);
  or_cond->rhs_->accept(this);
  return nullptr;
}

}  // namespace ast

}  // namespace dbtrain
