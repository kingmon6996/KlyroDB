#include <gtest/gtest.h>
#include "klyro/sql/lexer.hpp"

using namespace klyro::sql;

TEST(LexerTest, BasicTokens) {
    std::string sql = "SELECT id, name FROM users WHERE age > 18;";
    Lexer lexer(sql);
    auto tokens_res = lexer.tokenize();
    ASSERT_TRUE(tokens_res);
    
    auto tokens = tokens_res.value();
    ASSERT_EQ(tokens.size(), 11);
    
    EXPECT_EQ(tokens[0].type, TokenType::Select);
    EXPECT_EQ(tokens[1].type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].lexeme, "id");
    EXPECT_EQ(tokens[2].type, TokenType::Comma);
    EXPECT_EQ(tokens[3].type, TokenType::Identifier);
    EXPECT_EQ(tokens[4].type, TokenType::From);
    EXPECT_EQ(tokens[5].type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].type, TokenType::Where);
    EXPECT_EQ(tokens[7].type, TokenType::Identifier);
    EXPECT_EQ(tokens[8].type, TokenType::Greater);
    EXPECT_EQ(tokens[9].type, TokenType::IntegerLiteral);
    EXPECT_EQ(tokens[10].type, TokenType::Semicolon);
}

TEST(LexerTest, StringLiterals) {
    std::string sql = "SELECT 'hello ''world''' FROM t;";
    Lexer lexer(sql);
    auto tokens_res = lexer.tokenize();
    ASSERT_TRUE(tokens_res);
    
    auto tokens = tokens_res.value();
    EXPECT_EQ(tokens[1].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[1].lexeme, "hello 'world'");
}

TEST(LexerTest, QuotedIdentifiers) {
    std::string sql = "SELECT \"My Column\" FROM t;";
    Lexer lexer(sql);
    auto tokens_res = lexer.tokenize();
    ASSERT_TRUE(tokens_res);
    
    auto tokens = tokens_res.value();
    EXPECT_EQ(tokens[1].type, TokenType::QuotedIdentifier);
    EXPECT_EQ(tokens[1].lexeme, "My Column");
}
