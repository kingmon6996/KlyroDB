#ifndef KLYRO_SQL_AST_NODE_HPP
#define KLYRO_SQL_AST_NODE_HPP

#include "klyro/sql/source_location.hpp"

namespace klyro::sql::ast {

class ASTNode {
public:
    virtual ~ASTNode() = default;

    SourceLocation location() const noexcept { return m_location; }
    void set_location(SourceLocation loc) { m_location = loc; }

private:
    SourceLocation m_location;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_NODE_HPP
