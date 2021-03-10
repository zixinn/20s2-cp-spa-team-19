//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Parse Assign Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "nobody"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::CONST, "10"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* ass = p.parseAssignStmt();
    REQUIRE(ass->compareToken(new sp::Token(sp::Token::TokenType::ASSIGN, "=")));
    REQUIRE(ass->getName()->getVal() == "nobody");
    //REQUIRE(ass->compareExpr(new sp::Token(sp::Token::TokenType::CONST, "10")));
}

TEST_CASE("Parse StmtLst - Assign Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        // currently will not error when missing RHS this is intentional will fix after parsng RHS
        new sp::Token(sp::Token::TokenType::CONST, "55"),
        // currently will not error when missing ; this is intentional will fix after parsng RHS
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::NAME, "y"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::CONST, "100"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::NAME, "fixx"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::CONST, "949412"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::StmtLst* stmt_lst = p.parseStmtLst();

    REQUIRE((*stmt_lst).getStatements().size() == 3);

    std::vector<std::string> identifiers{ "x", "y", "fixx" };
    for (int i = 0; i < identifiers.size(); ++i) {
        auto id = identifiers[i];
        auto tmp_stmt = stmt_lst->getStatements()[i];
        ast::AssignStmt* ass = (ast::AssignStmt*)tmp_stmt;
        std::cout << ass->toString() << std::endl;
        REQUIRE(ass->compareToken(new sp::Token(sp::Token::TokenType::ASSIGN, "=")));
        REQUIRE(ass->getName()->getVal() == id);
        REQUIRE(ass->getIndex() == i + 1);
        //REQUIRE(false);
    }
}

TEST_CASE("Parse Assign Expr - Mid Test") {

    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::NAME, "v"),
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::NAME, "y"),
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::NAME, "z"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::NAME, "t"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* ass = p.parseAssignStmt();
    REQUIRE(ass->toString() == "(x) = (((v) + ((x) * (y))) + ((z) * (t)));");
}

TEST_CASE("Parse Assign Expr - With Paren Test") {

    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::LPAREN, "("),
        new sp::Token(sp::Token::TokenType::NAME, "y"),
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::CONST, "5"),
        new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::CONST, "3"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);
    Parser p = Parser(l);
    ast::AssignStmt* ass = p.parseAssignStmt();
    REQUIRE(ass->toString() == "(x) = (((y) + (5)) * (3));");
}

TEST_CASE("Parse Assign Expr - Cache Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "x = (y + 5) * 3;",
            "(((y) + (5)) * (3))",
        },
        {
            "xx = 1 + (2 + 3) + 4;",
            "(((1) + ((2) + (3))) + (4))",
        },
        {
            "yy = 2/(5+    5);",
            "((2) / ((5) + (5)))",
        },
        {
            "zz = 1;",
            "(1)",
        },
        {
            "aa = (1+(2));",
            "((1) + (2))",
        },
        {
            "bb = (b);",
            "(b)",
        },
        {
            "cc = (1) + ((2) + (3)) + (4);",
            "(((1) + ((2) + (3))) + (4))",
        },
        {
            "x = v + x * y + z * t;",
            "(((v) + ((x) * (y))) + ((z) * (t)))",
        },
    };

    for (int i = 0; i < tests.size(); ++i) {
        std::string input = std::get<0>(tests[i]);
        std::string expected = std::get<1>(tests[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            //ast::Stmt* stmt = p.parseStmt();
            ast::AssignStmt* ass = p.parseAssignStmt();
            REQUIRE(ass->getCachedExprString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            REQUIRE(false);
        }
    }
}
