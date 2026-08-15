#ifndef KLYRO_SQL_TOKEN_HPP
#define KLYRO_SQL_TOKEN_HPP

#include "klyro/sql/token_type.hpp"
#include "klyro/sql/source_location.hpp"
#include <string>

namespace klyro::sql {

struct Token {
    TokenType type{TokenType::Eof};
    std::string lexeme;
    SourceLocation location;

    Token() = default;
    Token(TokenType t, std::string l, SourceLocation loc)
        : type(t), lexeme(std::move(l)), location(loc) {}
};

} // namespace klyro::sql

#endif // KLYRO_SQL_TOKEN_HPP
