#ifndef KLYRO_EXECUTION_BINDER_BINDER_HPP
#define KLYRO_EXECUTION_BINDER_BINDER_HPP

#include "klyro/catalog/catalog.hpp"
#include "klyro/sql/ast/ast_visitor.hpp"
#include "klyro/sql/ast/statement.hpp"
#include "klyro/sql/ast/expression.hpp"
#include "klyro/sql/ast/select.hpp"
#include "klyro/sql/ast/insert.hpp"
#include "klyro/sql/ast/ast_visitor.hpp"
#include "klyro/execution/binder/bound_statement.hpp"
#include "klyro/execution/binder/name_resolver.hpp"

namespace klyro::execution::binder {

// Traverses the raw SQL AST and produces a BoundAST that has types resolved,
// catalog metadata attached, and semantic rules enforced.
class Binder {
public:
    Binder(catalog::Catalog* catalog) : m_resolver(catalog), m_catalog(catalog) {}

    // Main entry point
    Result<std::unique_ptr<BoundStatement>> bind_statement(const sql::ast::Statement* stmt);

private:
    NameResolver m_resolver;
    catalog::Catalog* m_catalog;

    // Statement binding
    Result<std::unique_ptr<BoundStatement>> bind_select(const sql::ast::SelectStatement* stmt);
    Result<std::unique_ptr<BoundStatement>> bind_insert(const sql::ast::InsertStatement* stmt);

    // Clause binding
    Result<std::unique_ptr<BoundTableRef>> bind_table_ref(const sql::ast::ASTNode* table_ref);
    
    // Expression binding
    Result<std::unique_ptr<BoundExpression>> bind_expression(const sql::ast::Expression* expr);
    Result<std::unique_ptr<BoundExpression>> bind_literal(const sql::ast::LiteralExpression* expr);
    Result<std::unique_ptr<BoundExpression>> bind_identifier(const sql::ast::IdentifierExpression* expr);
    Result<std::unique_ptr<BoundExpression>> bind_binary_op(const sql::ast::BinaryExpression* expr);
    
    // Type checking & casting
    Result<std::unique_ptr<BoundExpression>> enforce_type(std::unique_ptr<BoundExpression> expr, types::TypeID target_type);
    types::TypeID resolve_result_type(types::TypeID left, types::TypeID right);
};

} // namespace klyro::execution::binder

#endif // KLYRO_EXECUTION_BINDER_BINDER_HPP
