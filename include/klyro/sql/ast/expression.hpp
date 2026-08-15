#ifndef KLYRO_SQL_AST_EXPRESSION_HPP
#define KLYRO_SQL_AST_EXPRESSION_HPP

#include "klyro/sql/ast/ast_node.hpp"
#include <string>

namespace klyro::sql::ast {

class ASTVisitor;

class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
    
    // Accept method for Visitor pattern
    virtual void accept(ASTVisitor& visitor) const = 0;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_EXPRESSION_HPP
