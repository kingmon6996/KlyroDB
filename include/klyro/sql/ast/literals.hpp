#ifndef KLYRO_SQL_AST_LITERALS_HPP
#define KLYRO_SQL_AST_LITERALS_HPP

#include "klyro/sql/ast/expression.hpp"
#include "klyro/types/value.hpp"

namespace klyro::sql::ast {

class LiteralExpression : public Expression {
public:
    explicit LiteralExpression(types::Value val) : m_value(std::move(val)) {}
    const types::Value& value() const { return m_value; }
    
    void accept(ASTVisitor& visitor) const override;
private:
    types::Value m_value;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_LITERALS_HPP
