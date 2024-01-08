#include "optim.h"

#include <queue>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#include "utils/debug-print.hpp"
#include "exception/exceptions.h"
#include "exception/table_exceptions.h"
#include "oper/conditions/conditions.h"
#include "oper/conditions/join_condition.h"
#include "oper/conditions/logic_condition.h"
#include "oper/nodes.h"
#include "stats_manager.h"
#include "system/system_manager.h"
#include "utils/graph.h"
#include "utils/uf_set.h"

namespace dbtrain {

typedef std::pair<double, double> LUBound;

const string delimiter = ".";

Optimizer::Optimizer() { meta_ = &SystemManager::GetInstance(); }

Optimizer::~Optimizer() {}

std::any Optimizer::visit(Insert *insert) {
  RecordList record_list{};
  for (const auto &vals : insert->vals_list_) {
    Record *record = new Record();
    for (const auto &val : vals) {
      auto field = std::any_cast<Field *>(val->accept(this));
      record->PushBack(field);
    }
    record_list.push_back(record);
  }
  Table *table = meta_->GetTable(insert->table_name_);
  OperNode *plan = new InsertNode(table, record_list);
  return plan;
}

std::any Optimizer::visit(Delete *delete_) {
  string table_name = delete_->table_name_;
  // 初始化全表扫描结点
  OperNode *plan = new TableScanNode(meta_->GetTable(table_name));
  // 添加选择条件算子
  if (delete_->condition_ != nullptr) delete_->condition_->accept(this);

  auto it = table_filter_.find(table_name);
  if (it != table_filter_.end()) {
    FilterNode *node = new FilterNode(plan, table_filter_[table_name]);
    node->SetFilterTable(table_name);
    plan = node;
    table_filter_.erase(it);
  }
  // 返回删除结点
  plan = new DeleteNode(plan, meta_->GetTable(table_name));
  return plan;
}

std::any Optimizer::visit(Update *update) {
  string table_name = update->table_name_;
  // 初始化全表扫描结点
  OperNode *plan = new TableScanNode(meta_->GetTable(table_name));
  // 添加选择条件算子
  if (update->condition_ != nullptr) update->condition_->accept(this);
  auto it = table_filter_.find(table_name);
  if (it != table_filter_.end()) {
    FilterNode *node = new FilterNode(plan, table_filter_[table_name]);
    node->SetFilterTable(table_name);
    plan = node;
    table_filter_.erase(it);
  }
  // 添加SET条件
  vector<pair<int, Field *>> set_clauses{};
  for (const auto &set_clause : update->set_clauses_) {
    auto set_pair = std::any_cast<pair<string, Field *>>(set_clause->accept(this));
    auto col_idx = meta_->GetTable(table_name)->GetColumnIdx(set_pair.first);
    // 类型检测
    auto col_type = meta_->GetTable(table_name)->GetColumnType(col_idx);
    if (col_type != set_pair.second->GetType()) {
      throw InvalidUpdateTypeError(type2str[set_pair.second->GetType()], type2str[col_type]);
    }
    set_clauses.push_back({col_idx, set_pair.second});
  }
  // 返回更新结点
  plan = new UpdateNode(plan, meta_->GetTable(table_name), set_clauses);
  return plan;
}

std::any Optimizer::visit(Select *select) {
  auto uf_set = UFSet<string>(select->tables_);
  // 初始化全表扫描结点
  std::unordered_map<string, OperNode *> table_map{};
  for (const auto &table_name : select->tables_) {
    table_map[table_name] = new TableScanNode(meta_->GetTable(table_name));
    table_shift_[table_name] = 0;
  }
  // 添加选择条件算子
  if (select->condition_ != nullptr) select->condition_->accept(this);
  for (const auto &table_name : select->tables_) {
    auto it = table_filter_.find(table_name);
    if (it != table_filter_.end()) {
      FilterNode *node = new FilterNode(table_map[table_name], table_filter_[table_name]);
      node->SetFilterTable(table_name);
      table_map[table_name] = node;
      table_filter_.erase(it);
    }
  }

  // TODO: 基于基数优化JOIN的执行顺序
  // TIPS: 基础功能不需要考虑包含or情景的计划优化
  // TIPS: 首先需要完成直方图结构的相关函数
  // TIPS: 利用OperNode的Cost函数可以估计FilterNode和TableScanNode的基数
  // TIPS: 考虑到Join算子基数估计较为困难，基础功能仅做如下要求：
  // TIPS: (1) 估计join操作前table_map各个算子的基数
  // TIPS: (2) 按照join构建表的连接图（无向图位于utils/graph）
  // TIPS: (3) 按照算子基数，从基数最低的节点开始进行宽度优先搜索BFS，按照结点添加顺序生成join顺序
  // TIPS: 可以新建vector保存join顺序并修改LAB 3生成执行计划的顺序
  // TIPS: 注意，文档中给出了一个简单的例子
  // LAB 5 BEGIN
  class TWC{
   public:
    string name;
    string tree_name;
    double cost;
    bool operator<(const TWC& a) const
    {
      return cost < a.cost; //大顶堆
    }
  };

  struct tmp2
  {
    bool operator() (TWC a, TWC b)
    {
      return a.cost > b.cost; //
    }
  };
  std::unordered_map<string, double> cost_map{};
  std::vector<TWC> twc_list = {};
  for (auto iter:table_map){
    cost_map[iter.first] = iter.second->Cost();
    twc_list.push_back({iter.first, "", cost_map[iter.first]});
  }

  // 建图
  auto graph = UndirectedGraph<string>();
  for (auto & iter:table_filter_){
    if (dynamic_cast<JoinCondition *>(iter.second) != nullptr){
      auto union_table_name = iter.first;
      size_t found = union_table_name.rfind(delimiter);
      auto name_l = union_table_name.substr(0, found);
      auto name_r = union_table_name.substr(found + 1, union_table_name.length());
      graph.AddVertex(name_l);
      graph.AddVertex(name_r);
      graph.AddEdge(name_l, name_r);
    }
  }
  LAB5Print("the graph has ", graph.NodeNum(), " nodes");

  // 找到最小值的map name
  auto min_twc = * std::min_element(twc_list.begin(), twc_list.end());
  std::priority_queue<TWC, std::vector<TWC>, tmp2> q;
  q.push(min_twc);
  std::set<string> tree_nodes;
//  LAB5Print("ming_twc", min_twc.name, " ", min_twc.cost);
  while ( tree_nodes.size() < graph.NodeNum()){

    while (tree_nodes.find(q.top().name) != tree_nodes.end()) q.pop();
//    LAB5Print("choosing node:", q.top().name, " cost:", q.top().cost);
    auto selected_name = q.top().name;
    auto selected_tree_name = q.top().tree_name;
    tree_nodes.insert(selected_name);
    q.pop();
    if (!q.top().tree_name.empty()){
      const string& name_l = selected_tree_name;
      const string& name_r = selected_name;
      for (auto & iter:table_filter_) {
        auto cond = dynamic_cast<JoinCondition *>(iter.second);
        if (cond != nullptr and (iter.first == name_l + delimiter + name_r or
                                 iter.first == name_r + delimiter + name_l)){
//          LAB5Print("uning table name:", iter.first);
          cond->LeftShift(table_shift_[name_l]);
          cond->RightShift(table_shift_[name_r]);
          auto root_l = uf_set.Find(name_l);
          auto root_r = uf_set.Find(name_r);
          auto join_node = new JoinNode(table_map[root_l], table_map[root_r], cond);

          if (table_record_len.find(root_l) == table_record_len.end()){
            table_record_len[root_l] = meta_->GetTable(root_l)->GetColumnSize();
          }
          if (table_record_len.find(root_r) == table_record_len.end()){
            table_record_len[root_r] = meta_->GetTable(root_r)->GetColumnSize();
          }

          auto rs = uf_set.FindAll(name_r);
          for (auto & name:rs){
            table_shift_[name] += table_record_len[root_l];
          }

          uf_set.Union(name_l, name_r);
          auto root = uf_set.Find(name_r);
          table_map[root] = join_node;
          table_record_len[root] =  table_record_len[root_r] + table_record_len[root_l];

        }
      }
    }
    for (auto &neighbor: graph.Adjace(selected_name)){
      if (tree_nodes.find(neighbor) != tree_nodes.end()) continue;
//      LAB5Print("inserting:", neighbor, " tree_node:", selected_name);
      q.push({neighbor, selected_name, cost_map[neighbor]});
    }
  }


  string first_table = uf_set.Find(select->tables_[0]);

  // 检验所有的表都已经被JOIN
  for (const auto &table_name : select->tables_) {
    if (uf_set.Find(table_name) != first_table) throw TableNotJoinedError(table_name);
  }

  // 添加投影算子
  OperNode *plan = table_map[first_table];
  if (!select->cols_.empty()) {
    vector<int> proj_idxs{};
    for (const auto &col : select->cols_) {
      int offset = table_shift_[col->table_name_];
      LAB5Print("SEL project: table:", col->table_name_, " colname:", col->col_name_,
                " offset:", offset, " col_idx:", meta_->GetTable(col->table_name_)->GetColumnIdx(col->col_name_));
      proj_idxs.push_back(offset + meta_->GetTable(col->table_name_)->GetColumnIdx(col->col_name_));
    }
    plan = new ProjectNode(plan, proj_idxs);
  }
  // 返回选择算子
  plan = new SelectNode(plan);
  return plan;
}

std::any Optimizer::visit(LessConditionNode *cond) {
  // 小于选择条件
  string table_name = cond->col_->table_name_;
  string col_name = cond->col_->col_name_;
  int col_idx = meta_->GetTable(table_name)->GetColumnIdx(col_name);
  Field *field = std::any_cast<Field *>(cond->value_->accept(this));
  Condition *condition = new LessCondition(col_idx, field);
  if (table_filter_.find(table_name) == table_filter_.end()) {
    table_filter_[table_name] = condition;
    condition = nullptr;
  }
  std::pair<string, Condition *> cpair{table_name, condition};
  return cpair;
}

std::any Optimizer::visit(EqualConditionNode *cond) {
  // 等值选择条件
  string table_name = cond->col_->table_name_;
  string col_name = cond->col_->col_name_;
  int col_idx = meta_->GetTable(table_name)->GetColumnIdx(col_name);
  Field *field = std::any_cast<Field *>(cond->value_->accept(this));
  Condition *condition = new EqualCondition(col_idx, field);
  if (table_filter_.find(table_name) == table_filter_.end()) {
    table_filter_[table_name] = condition;
    condition = nullptr;
  }
  std::pair<string, Condition *> cpair{table_name, condition};
  return cpair;
}

std::any Optimizer::visit(GreaterConditionNode *cond) {
  // 大于选择条件
  string table_name = cond->col_->table_name_;
  string col_name = cond->col_->col_name_;
  int col_idx = meta_->GetTable(table_name)->GetColumnIdx(col_name);
  Field *field = std::any_cast<Field *>(cond->value_->accept(this));
  Condition *condition = new GreaterCondition(col_idx, field);
  if (table_filter_.find(table_name) == table_filter_.end()) {
    table_filter_[table_name] = condition;
    condition = nullptr;
  }
  std::pair<string, Condition *> cpair{table_name, condition};
  return cpair;
}

std::any Optimizer::visit(JoinConditionNode *join_condition) {
  // TODO: （高级功能）连接运算左右算子选择
  // TIPS: 注意，部分连接运算与左右算子顺序相关，需要调整正确的执行数据
  // TIPS: 注意，在基础功能测例中不检查，但是建议使用了顺序相关JOIN算法的同学正确调整JOIN顺序
  // LAB 5 BEGIN
  // LAB 5 END

  // TODO: 解析连接条件
  // TIPS: 将 JoinCondition 添加到 table_filter_ 中
  // TIPS: 可以将两个表名组合成一个新的表名，以 delimiter 变量为分隔符，作为 table_filter_ 的 key
  // LAB 4 BEGIN
  string table_name_l = join_condition->lhs_->table_name_;
  string col_name_l = join_condition->lhs_->col_name_;
  string table_name_r = join_condition->rhs_->table_name_;
  string col_name_r = join_condition->rhs_->col_name_;
  auto col_idx_l = meta_->GetTable(table_name_l)->GetColumnIdx(col_name_l);
  auto col_idx_r = meta_->GetTable(table_name_r)->GetColumnIdx(col_name_r);

  auto table_name = table_name_l + delimiter + table_name_r;
  LAB4Print("Optimizer::visit(JoinConditionNode>", table_name);
  Condition *condition = new JoinCondition(col_idx_l, col_idx_r);
  if (table_filter_.find(table_name) == table_filter_.end()) {
    table_filter_[table_name] = condition;
    //    condition = nullptr;
  }
  std::pair<string, Condition *> cpair{table_name, condition};
  return cpair;
  // LAB 4 END
}

std::any Optimizer::visit(AndConditionNode *and_condition) {
  // TIPS: 调用 and_condition lhs_ 和 rhs_ 的 accept 函数进行访问
  // TIPS: parser 会将多个 AND 条件解析为左深树
  // TIPS: 将 Condition 记录在 table_filter_ 中
  // TIPS: 函数需要添加返回值，否则可能出现段错误
  // LAB 4 BEGIN
  auto res_l = and_condition->lhs_->accept(this);
  auto res_r = and_condition->rhs_->accept(this);
  try{
    auto cpair_l = std::any_cast<std::pair<string, Condition *>>(res_l);
    auto cpair_r = std::any_cast<std::pair<string, Condition *>>(res_r);
    auto table_name_l = cpair_l.first;
    auto table_name_r = cpair_r.first;

    if (table_name_l == table_name_r){
      LAB4Print("Optimizer::visit(AndConditionNode>", table_name_r);
      auto & table_name = table_name_r;
      auto cond_l = cpair_l.second;
      auto cond_r = cpair_r.second;
      vector<Condition *> conds = {cond_l, cond_r};

      if (table_filter_.find(table_name) != table_filter_.end()) {
        conds.push_back(table_filter_[table_name]);
      }

      Condition *condition = new AndCondition(conds);
      if (table_filter_.find(table_name) == table_filter_.end()) {
        table_filter_[table_name] = condition;
        condition = nullptr;
      }else{
        table_filter_[table_name] = condition;
      }
      std::pair<string, Condition *> cpair{table_name, condition};
      return cpair_l;
    } else {
      LAB4Print("Optimizer::visit(AndConditionNode>", table_name_l, " ", table_name_r);
      return nullptr;
    }
  }
  catch(const std::bad_any_cast& e){
    return nullptr;
  }

  // LAB 4 END
}

std::any Optimizer::visit(OrConditionNode *or_condition) {
  // TIPS: 与 AndCondition 流程相同
  // LAB 4 BEGIN
  auto res_l = or_condition->lhs_->accept(this);
  auto res_r = or_condition->rhs_->accept(this);
  try{
    auto cpair_l = std::any_cast<std::pair<string, Condition *>>(res_l);
    auto cpair_r = std::any_cast<std::pair<string, Condition *>>(res_r);
    auto table_name_l = cpair_l.first;
    auto table_name_r = cpair_r.first;

    if (table_name_l == table_name_r){
      LAB4Print("OrConditionNode>", table_name_r);
      auto & table_name = table_name_r;
      auto cond_l = cpair_l.second;
      auto cond_r = cpair_r.second;
      vector<Condition *> conds = {cond_l, cond_r};

      if (table_filter_.find(table_name) != table_filter_.end()) {
        conds.push_back(table_filter_[table_name]);
      }

      Condition *condition = new OrCondition(conds);
      if (table_filter_.find(table_name) == table_filter_.end()) {
        table_filter_[table_name] = condition;
        condition = nullptr;
      }else{
        table_filter_[table_name] = condition;
      }
      std::pair<string, Condition *> cpair{table_name, condition};
      return cpair_l;
    } else {
      LAB4Print("OrConditionNode>", table_name_l, " ", table_name_r);
      return nullptr;
    }
  }
  catch(const std::bad_any_cast& e){
    return nullptr;
  }

  // LAB 4 END
}

}  // namespace dbtrain
