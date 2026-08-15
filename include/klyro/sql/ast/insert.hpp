#ifndef KLYRO_SQL_AST_INSERT_HPP
#define KLYRO_SQL_AST_INSERT_HPP

#include "klyro/sql/ast/statement.hpp"
#include "klyro/sql/ast/expression.hpp"
#include <vector>
#include <memory>
#include <string>

namespace klyro::sql::ast {

class InsertStatement : public Statement {
public:
    InsertStatement(std::string table_name, 
                    std::vector<std::string> columns,
                    std::vector<std::vector<std::unique_ptr<Expression>>> values)
        : m_table_name(std::move(table_name)), 
          m_columns(std::move(columns)),
          m_values(std::move(values)) {}

    const std::string& table_name() const { return m_table_name; }
    const std::vector<std::string>& columns() const { return m_columns; }
    const std::vector<std::vector<std::unique_ptr<Expression>>>& values() const { return m_values; }
    
    void accept(ASTVisitor& visitor) const override;
private:
    std::string m_table_name;
    std::vector<std::string> m_columns; // Can be empty if omitting column list
    std::vector<std::vector<std::unique_ptr<Expression>>> m_values;
};

} // namespace klyro::sql::ast

#endif // KLYRO_SQL_AST_INSERT_HPP
