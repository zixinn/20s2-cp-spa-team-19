#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/Token.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Parse Print Test") {
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, "xyz"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::PrintStmt* ass = p.parsePrintStmt();
    REQUIRE(ass->compareToken(new sp::Token(sp::Token::TokenType::PRINT, "print")));
    REQUIRE(ass->getName()->getVal() == "xyz");
}

TEST_CASE("Parse StmtLst - Print Test") {
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, "xxy"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, "aBcD"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, "GME"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),    // lexer would identify as keyword
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::StmtLst* stmt_lst = p.parseStmtLst();

    REQUIRE((*stmt_lst).getStatements().size() == 4);

    std::vector<STRING> identifiers{ "xxy", "aBcD", "GME", "print" };
    for (int i = 0; i < identifiers.size(); ++i) {
        auto id = identifiers[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        ast::PrintStmt* calls = (ast::PrintStmt*)tmp_stmt;
        REQUIRE(calls->compareToken(new sp::Token(sp::Token::TokenType::PRINT, "print")));
        REQUIRE(calls->getName()->compareToken(new sp::Token(sp::Token::TokenType::NAME, id)));
        REQUIRE(calls->getName()->getVal() == id);
        REQUIRE(calls->getIndex() == i + 1);
    }
}

// keywords are allowed, lexer will not detect as name, but are still legal
TEST_CASE("Parse StmtLst - Print - Keyword Test") {
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, "sun"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::PROC, "procedure"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::READ, "read"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::CALL, "call"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::WHILE, "while"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::IF, "if"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::THEN, "then"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::ELSE, "else"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::StmtLst* stmt_lst = p.parseStmtLst();

    REQUIRE((*stmt_lst).getStatements().size() == 9);

    std::vector<STRING> identifiers{ "sun", "procedure", "read", "print", "call", "while",
                                          "if", "then", "else"};
    for (int i = 0; i < identifiers.size(); ++i) {
        auto id = identifiers[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        ast::PrintStmt* calls = (ast::PrintStmt*)tmp_stmt;
        REQUIRE(calls->compareToken(new sp::Token(sp::Token::TokenType::PRINT, "print")));
        INFO("Token expected: " + id + ", Actual literal: " + calls->getName()->getTokenLiteral());
        REQUIRE(calls->getName()->compareToken(new sp::Token(sp::Token::TokenType::NAME, id)));
        REQUIRE(calls->getName()->getVal() == id);
        REQUIRE(calls->getIndex() == i + 1);
    }

    // testing toString()
    std::vector<STRING> expected_strings{
            "print (sun);",
            "print (procedure);",
            "print (read);",
            "print (print);",
            "print (call);",
            "print (while);",
            "print (if);",
            "print (then);",
            "print (else);",
    };
    for (int i = 0; i < identifiers.size(); ++i) {
        auto str = expected_strings[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        REQUIRE(str == tmp_stmt->toString());
    }
}
