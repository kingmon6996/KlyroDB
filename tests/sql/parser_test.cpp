#include <gtest/gtest.h>
#include "klyro/sql/parser.hpp"
#include "klyro/sql/ast/ast_formatter.hpp"
#include "klyro/sql/ast/select.hpp"
#include "klyro/sql/ast/expressions.hpp"

using namespace klyro::sql;
using namespace klyro::sql::ast;

TEST(ParserTest, ParseSelectStatement) {
    std::string sql = "SELECT id, name AS username FROM users u WHERE age > 18;";
    Parser parser(sql);
    auto stmt_res = parser.parse_statement();
    ASSERT_TRUE(stmt_res);
    
    auto stmt = std::move(stmt_res.value());
    SelectStatement* sel = dynamic_cast<SelectStatement*>(stmt.get());
    ASSERT_NE(sel, nullptr);
    
    EXPECT_EQ(sel->projection().size(), 2);
    EXPECT_EQ(sel->projection()[1].alias, "username");
    
    EXPECT_EQ(sel->from_tables().size(), 1);
    EXPECT_EQ(sel->from_tables()[0].table_name, "users");
    EXPECT_EQ(sel->from_tables()[0].alias, "u");
    
    ASSERT_NE(sel->where_clause(), nullptr);
    
    const BinaryExpression* where_bin = dynamic_cast<const BinaryExpression*>(sel->where_clause());
    ASSERT_NE(where_bin, nullptr);
    EXPECT_EQ(where_bin->op(), TokenType::Greater);
}

TEST(ParserTest, ParseExpressionPrecedence) {
    std::string sql = "SELECT 1 + 2 * 3 FROM dual;";
    Parser parser(sql);
    auto stmt_res = parser.parse_statement();
    ASSERT_TRUE(stmt_res);
    
    SelectStatement* sel = dynamic_cast<SelectStatement*>(stmt_res.value().get());
    const BinaryExpression* expr = dynamic_cast<const BinaryExpression*>(sel->projection()[0].expr.get());
    
    // Root should be +
    ASSERT_EQ(expr->op(), TokenType::Plus);
    
    // Right should be *
    const BinaryExpression* right = dynamic_cast<const BinaryExpression*>(expr->right());
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->op(), TokenType::Star);
}

TEST(ParserTest, ParseCountStar) {
    std::string sql = "SELECT COUNT(*) FROM users;";
    Parser parser(sql);
    auto stmt_res = parser.parse_statement();
    ASSERT_TRUE(stmt_res);
    
    SelectStatement* sel = dynamic_cast<SelectStatement*>(stmt_res.value().get());
    const FunctionCallExpression* func = dynamic_cast<const FunctionCallExpression*>(sel->projection()[0].expr.get());
    
    ASSERT_NE(func, nullptr);
    EXPECT_EQ(func->name(), "COUNT");
    // Handled syntactically in the parser, may have no args in ast or one `*` arg.
}
