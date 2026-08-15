#include "klyro/sql/ast/expressions.hpp"
#include "klyro/sql/ast/ast_visitor.hpp"

namespace klyro::sql::ast {

void IdentifierExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
void BinaryExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
void UnaryExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
void FunctionCallExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
void ParameterExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

} // namespace klyro::sql::ast
