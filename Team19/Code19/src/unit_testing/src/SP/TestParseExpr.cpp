//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Parse Name Expr Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "foobar"),
        //new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Expr* expr = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
    try {
        auto vn = (ast::VarName*)expr;
        REQUIRE(vn->getToken()->compare(stubTokens[0]));
        REQUIRE(vn->val == stubTokens[0]->getLiteral());
    }
    catch (const std::string& ex) {
        INFO("failed with exception: " + ex);
        REQUIRE(false);
    }
}

TEST_CASE("Parse ConstVal Expr Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::CONST, "55"),
        //new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Expr* expr = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
    try {
        auto cv = (ast::ConstVal*)expr;
        REQUIRE(cv->getToken()->compare(stubTokens[0]));
        REQUIRE(cv->val == 55);
    }
    catch (const std::string& ex) {
        INFO("failed with exception: " + ex);
        REQUIRE(false);
    }
}

TEST_CASE("Parse Infix Expr - Basic Plus Test") {

    std::vector<sp::Token*> operators{
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::MINUS, "-"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::DIV, "/"),
        new sp::Token(sp::Token::TokenType::MOD, "%"),
    };

    std::vector<std::string> str_rep{
        "(11 + 22)",
        "(11 - 22)",
        "(11 * 22)",
        "(11 / 22)",
        "(11 % 22)",
    };

    for (int i = 0; i < operators.size(); i++) {
        auto op = operators[i];
        auto expr_str = str_rep[i];
        std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::CONST, "11"),
            op,
            new sp::Token(sp::Token::TokenType::CONST, "22"),
        };

        auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
        Parser p = Parser(l);
        ast::Expr* expr = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
        //REQUIRE(false);
        try {
            auto ie = (ast::InfixExpr*)expr;
            REQUIRE(ie->toString() == expr_str);
            REQUIRE(ie->compareToken(op));
            auto left = (ast::ConstVal*)ie->getLeft();
            auto right = (ast::ConstVal*)ie->getRight();
            REQUIRE(left->getToken()->compare(stubTokens[0]));
            REQUIRE(left->val == 11);
            REQUIRE(right->getToken()->compare(stubTokens[2]));
            REQUIRE(right->val == 22);
        }
        catch (std::string& ex) {
            INFO("failed with exception: " + ex);
            REQUIRE(false);
        }
    }
}

TEST_CASE("Parse Infix Expr - Mid Test") {

    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "v"),
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::NAME, "y"),
        new sp::Token(sp::Token::TokenType::PLUS, "+"),
        new sp::Token(sp::Token::TokenType::NAME, "z"),
        new sp::Token(sp::Token::TokenType::TIMES, "*"),
        new sp::Token(sp::Token::TokenType::NAME, "t"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::Expr* expr = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
    REQUIRE(expr->toString() == "((v + (x * y)) + (z * t))");
}

// test use of Keywords in Expressions
// NOTE: for some reason, Lexer::Tokenize expects a RPAREN ) or SEMICOLON ; to terminate string
TEST_CASE("ParseLexer2 Expr - Keywords Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "1 + print;",
            "(1 + print)",
        },
        {
            "call;",
            "call",
        },
        {
            "(5 + read) * if / then + else;",
            "((((5 + read) * if) / then) + else)",
        },
    };

    for (int i = 0; i < tests.size(); ++i) {
        std::string input = std::get<0>(tests[i]);
        std::string expected = std::get<1>(tests[i]);

        // begin ritual to Summon Parser
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        // Parser now ready for use

        ast::Expr* ce = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
        REQUIRE(ce->toString() == expected);
    }
}


// no this doesnt work, parseExpr doesnt check entire string it seems
//TEST_CASE("ParseLexer2 Expr - Exception Test") {
//
//    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
//    std::vector<std::pair<std::string, std::string>> tests{
//        {
//            "1 && print;",
//            "1 && print",
//        },
//    };
//
//    for (int i = 0; i < tests.size(); ++i) {
//        std::string input = std::get<0>(tests[i]);
//        std::string expected = std::get<1>(tests[i]);
//
//        // begin ritual to Summon Parser
//        std::vector<sp::Token> actual_tok;
//        std::vector<sp::Token*> tok_ptrs;
//        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
//        auto l = new LexerStub(tok_ptrs);
//        auto p = Parser(l);
//        // Parser now ready for use
//
//        try {
//            ast::Expr* ce = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
//            REQUIRE(ce->toString() == expected);
//            REQUIRE(false);
//        }
//        catch (sp::ParseExprException& ex) {
//            REQUIRE(true);
//        }
//    }
//}


