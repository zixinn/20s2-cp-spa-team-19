#include <string>
#include <vector>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/Token.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Basic Parse Call Test") {
    //sp::Token::TokenType tok_type = sp::Token::TokenType::PLUS;
    //REQUIRE(false);
}

TEST_CASE("Parse Call Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::NAME, "star"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        //new sp::Token(sp::Token::TokenType::CALL, "call"),
        //new sp::Token(sp::Token::TokenType::NAME, "sun"),
        //new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::CallStmt* ass = p.parseCallStmt();
    REQUIRE(ass->compareToken(new sp::Token(sp::Token::TokenType::CALL, "call")));
    REQUIRE(ass->getName()->getVal() == "star");
}

TEST_CASE("Parse StmtLst - Call Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::NAME, "sun"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::NAME, "moon"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"), // keywords are allowed, lexer will not detect as name
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::StmtLst* stmt_lst = p.parseStmtLst();

    REQUIRE((*stmt_lst).getStatements().size() == 3);

    std::vector<std::string> identifiers{ "sun", "moon", "procedure" };
    for (int i = 0; i < identifiers.size(); ++i) {
        auto id = identifiers[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        ast::CallStmt* calls = (ast::CallStmt*)tmp_stmt;
        REQUIRE(calls->compareToken(new sp::Token(sp::Token::TokenType::CALL, "call")));
        REQUIRE(calls->getName()->compareToken(new sp::Token(sp::Token::TokenType::NAME, id)));
        REQUIRE(calls->getName()->getVal() == id);
        REQUIRE(calls->getIndex() == i + 1);
        //REQUIRE(false);
    }
}

// keywords are allowed, lexer will not detect as name, but are still legal
TEST_CASE("Parse StmtLst - Call - Keyword Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::NAME, "sun"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"), 
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::READ, "read"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::PRINT, "print"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::WHILE, "while"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::IF, "if"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::THEN, "then"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::ELSE, "else"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::StmtLst* stmt_lst = p.parseStmtLst();

    REQUIRE((*stmt_lst).getStatements().size() == 9);

    std::vector<std::string> identifiers{ "sun", "procedure", "read", "print", "call", "while", 
        "if", "then", "else"};
    for (int i = 0; i < identifiers.size(); ++i) {
        auto id = identifiers[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        ast::CallStmt* calls = (ast::CallStmt*)tmp_stmt;
        REQUIRE(calls->compareToken(new sp::Token(sp::Token::TokenType::CALL, "call")));
        INFO("Token expected: " + id + ", Actual literal: " + calls->getName()->getTokenLiteral());
        REQUIRE(calls->getName()->compareToken(new sp::Token(sp::Token::TokenType::NAME, id)));
        REQUIRE(calls->getName()->getVal() == id);
        REQUIRE(calls->getIndex() == i + 1);
        //REQUIRE(false);
    }
}


