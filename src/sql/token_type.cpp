#include "klyro/sql/token_type.hpp"

namespace klyro::sql {

std::string_view to_string(TokenType type) {
    switch (type) {
        case TokenType::Eof: return "EOF";
        case TokenType::Identifier: return "Identifier";
        case TokenType::Select: return "SELECT";
        case TokenType::From: return "FROM";
        case TokenType::Where: return "WHERE";
        // ... truncated for brevity. For a robust printer, map all types.
        default: return "Unknown Token";
    }
}

} // namespace klyro::sql
