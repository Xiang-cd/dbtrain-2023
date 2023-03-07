#include "logic_condition.h"

namespace dbtrain {

AndCondition::AndCondition(const vector<Condition *> &conds) : conds_(conds) {}

AndCondition::~AndCondition() {
  for (const auto &cond : conds_)
    if (cond != nullptr) delete cond;
}

bool AndCondition::Fit(Record *record) const {
  for (const auto &cond : conds_)
    if (cond != nullptr)
      if (!cond->Fit(record)) return false;
  return true;
}

void AndCondition::PushBack(Condition *cond) { conds_.push_back(cond); }

vector<Condition *> AndCondition::GetConditions() const { return conds_; }

void AndCondition::Display() const {
  for (int i = 0; i < conds_.size() - 1; ++i) {
    conds_[i]->Display();
    printf("&");
  }
  conds_[conds_.size() - 1]->Display();
}

OrCondition::OrCondition(const vector<Condition *> &conds) : conds_(conds) {}

OrCondition::~OrCondition() {
  for (const auto &cond : conds_)
    if (cond != nullptr) delete cond;
}

void OrCondition::PushBack(Condition *cond) { conds_.push_back(cond); }

bool OrCondition::Fit(Record *record) const {
  for (const auto &cond : conds_)
    if (cond != nullptr)
      if (cond->Fit(record)) return true;
  return false;
}

void OrCondition::Display() const {
  for (int i = 0; i < conds_.size() - 1; ++i) {
    conds_[i]->Display();
    printf("|");
  }
  conds_[conds_.size() - 1]->Display();
}

NotCondition::NotCondition(Condition *cond) : cond_(cond) {}

NotCondition::~NotCondition() {
  if (cond_ != nullptr) delete cond_;
}

bool NotCondition::Fit(Record *record) const {
  if (cond_ == nullptr) return false;
  return !cond_->Fit(record);
}

void NotCondition::Display() const {
  printf("!");
  cond_->Display();
}

}  // namespace dbtrain
