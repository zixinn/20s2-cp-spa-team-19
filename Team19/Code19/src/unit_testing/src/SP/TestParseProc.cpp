#include <string>
#include <iostream>
#include <vector>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/Token.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("Parse Proc Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::NAME, "star"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::PRINT, "print"),
        new sp::Token(sp::Token::TokenType::NAME, "sun"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::READ, "read"),
        new sp::Token(sp::Token::TokenType::NAME, "ttt"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        //parse stmtlst used to terminate at EOF
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),

    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Proc* proc = p.parseProc();
    REQUIRE(proc->compareToken(new sp::Token(sp::Token::TokenType::PROC, "procedure")));
    REQUIRE(proc->getName()->getVal() == "star");

    ast::StmtLst* stmt_lst = proc->getStmtLst();
    REQUIRE((*stmt_lst).getStatements().size() == 2);
    
    std::string expectedString = "{\n    print sun;\n    read ttt;\n}\n";
    REQUIRE(stmt_lst->toString() == expectedString);
}

TEST_CASE("Parse Prog - Proc Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::NAME, "star"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::CONST, "1"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::NAME, "moon"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::ASSIGN, "="),
        new sp::Token(sp::Token::TokenType::CONST, "1"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Program* prog = p.parseProgram();
    std::vector<ast::Proc*> procs = prog->getProcedures();
    std::vector<std::string> identifiers{ "star", "moon" };

    for (int i = 0; i < identifiers.size(); i++) {
        ast::Proc* proc = procs[i];
        REQUIRE(proc->compareToken(new sp::Token(sp::Token::TokenType::PROC, "procedure")));
        REQUIRE(proc->getName()->getVal() == identifiers[i]);

        ast::StmtLst* stmt_lst = proc->getStmtLst();
        REQUIRE((*stmt_lst).getStatements().size() == 1);

        std::string expectedString = "{\n    x = 1;\n}\n";
        REQUIRE(stmt_lst->toString() == expectedString);

    }

}


TEST_CASE("Parse Prog - Proc - Keyword Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::READ, "read"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::PRINT, "print"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::CALL, "call"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::WHILE, "while"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::IF, "if"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::THEN, "then"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::ELSE, "else"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Program* prog = p.parseProgram();
    std::vector<ast::Proc*> procs = prog->getProcedures();
    std::vector<std::string> identifiers{ "procedure", "read", "print", "call", "while",
        "if", "then", "else" };
    
    for (int i = 0; i < identifiers.size(); i++) {
        ast::Proc* proc = procs[i];
        REQUIRE(proc->compareToken(new sp::Token(sp::Token::TokenType::PROC, "procedure")));
        REQUIRE(proc->getName()->getVal() == identifiers[i]);

        ast::StmtLst* stmt_lst = proc->getStmtLst();
        REQUIRE((*stmt_lst).getStatements().size() == 0);

        std::string expectedString = "{\n}\n";
        REQUIRE(stmt_lst->toString() == expectedString);

    }

}

