#include "klyro/sql/ast/literals.hpp"
#include "klyro/sql/ast/ast_visitor.hpp"

namespace klyro::sql::ast {

void LiteralExpression::accept(ASTVisitor& visitor) const {
    visitor.visit(*this);
}

} // namespace klyro::sql::ast
