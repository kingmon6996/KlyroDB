#ifndef KLYRO_EXECUTION_PLANNER_PLANNER_HPP
#define KLYRO_EXECUTION_PLANNER_PLANNER_HPP

#include "klyro/execution/binder/bound_statement.hpp"
#include "klyro/execution/planner/logical_operator.hpp"
#include "klyro/core/result.hpp"

namespace klyro::execution::planner {

// Translates a BoundStatement into a LogicalOperator tree
class Planner {
public:
    Planner() = default;

    Result<std::unique_ptr<LogicalOperator>> plan_statement(const binder::BoundStatement* stmt);

private:
    Result<std::unique_ptr<LogicalOperator>> plan_select(const binder::BoundSelect* stmt);
    Result<std::unique_ptr<LogicalOperator>> plan_insert(const binder::BoundInsert* stmt);
};

} // namespace klyro::execution::planner

#endif // KLYRO_EXECUTION_PLANNER_PLANNER_HPP
