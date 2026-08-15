#ifndef KLYRO_SQL_PARSER_HPP
#define KLYRO_SQL_PARSER_HPP

#include "klyro/sql/lexer.hpp"
#include "klyro/sql/ast/statement.hpp"
#include "klyro/sql/ast/expression.hpp"
#include "klyro/core/status.hpp"
#include "klyro/core/result.hpp"
#include <memory>
#include <string_view>

namespace klyro::sql {

enum class Precedence {
    None = 0,
    Assignment, // =
    Or,         // OR
    And,        // AND
    Equality,   // == !=
    Comparison, // < > <= >=
    Term,       // + -
    Factor,     // * /
    Unary,      // ! -
    Call,       // . ()
    Primary
};

class Parser {
public:
    explicit Parser(std::string_view sql);
    Result<std::unique_ptr<ast::Statement>> parse_statement();

private:
    std::vector<Token> m_tokens;
    std::size_t m_current{0};

    const Token& peek() const;
    const Token& previous() const;
    bool is_at_end() const;
    bool match(TokenType type);
    bool check(TokenType type) const;
    Token advance();
    Result<Token> consume(TokenType type, const std::string& message);

    // Expression parsing (Pratt Parser)
    Result<std::unique_ptr<ast::Expression>> parse_expression(Precedence precedence = Precedence::Assignment);
    Result<std::unique_ptr<ast::Expression>> parse_prefix(const Token& token);
    Result<std::unique_ptr<ast::Expression>> parse_infix(const Token& token, std::unique_ptr<ast::Expression> left);
    Precedence get_precedence(TokenType type) const;

    // Statement parsing
    Result<std::unique_ptr<ast::Statement>> parse_select();
};

} // namespace klyro::sql

#endif // KLYRO_SQL_PARSER_HPP
