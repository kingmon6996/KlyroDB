#ifndef KLYRO_SQL_AST_STATEMENT_HPP
#define KLYRO_SQL_AST_STATEMENT_HPP

#include "klyro/sql/ast/ast_node.hpp"

namespace klyro::sql::ast {

class ASTVisitor;

class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
    
    // Visitor pattern
    virtual void accept(ASTVisitor& visitor) const = 0;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_STATEMENT_HPP
