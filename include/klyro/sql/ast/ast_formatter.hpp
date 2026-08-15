#ifndef KLYRO_SQL_AST_FORMATTER_HPP
#define KLYRO_SQL_AST_FORMATTER_HPP

#include "klyro/sql/ast/ast_visitor.hpp"
#include "klyro/sql/ast/statement.hpp"
#include "klyro/sql/ast/expression.hpp"
#include <string>
#include <sstream>

namespace klyro::sql::ast {

class ASTFormatter : public ASTVisitor {
public:
    std::string format(const Statement& stmt);
    std::string format(const Expression& expr);

    void visit(const IdentifierExpression& node) override;
    void visit(const BinaryExpression& node) override;
    void visit(const UnaryExpression& node) override;
    void visit(const FunctionCallExpression& node) override;
    void visit(const ParameterExpression& node) override;
    void visit(const LiteralExpression& node) override;
    
    void visit(const SelectStatement& node) override;
    void visit(const InsertStatement& node) override;
    void visit(const CreateTableStatement& node) override;

private:
    std::stringstream m_ss;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_FORMATTER_HPP
