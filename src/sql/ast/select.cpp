#include "klyro/sql/ast/select.hpp"
#include "klyro/sql/ast/ast_visitor.hpp"

namespace klyro::sql::ast {

void SelectStatement::accept(ASTVisitor& visitor) const {
    visitor.visit(*this);
}

} // namespace klyro::sql::ast
