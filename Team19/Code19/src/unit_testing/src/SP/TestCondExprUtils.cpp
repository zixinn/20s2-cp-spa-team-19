#pragma once
//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"
#include "SP/CondExprUtils.h"

#include "catch.hpp"
using namespace std;

// test for some bracketings assign expr
// NOTE: for some reason, Lexer::Tokenize expects a RPAREN ) or SEMICOLON ; to terminate string
TEST_CASE("CondExprUtils - Test") {
    std::string input = "0 1 2 3 4 5 6 6 8 9";


    //
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    //

    std::vector<sp::Token*> out_ptrs;
    try {
	    CondExprUtils::VectorSlice(tok_ptrs, out_ptrs, 2, 4);
        auto result = CondExprUtils::VectorToString(out_ptrs);
        REQUIRE(result == "2 3 4");
    }
    catch (sp::UtilsException& ex) {
        INFO(ex.what());
        REQUIRE(false);
    }

}

TEST_CASE("CondExprUtils - ParseRelExpr Test") {

    std::vector<std::tuple<std::string, int, std::string>> tests{
        {
            "(flag >= b)", 2,   // >= at index 2
            "( BOOL )",
        },
        {
            "(flag >= b + 3 * 4)", 2,   // >= at index 2
            "( BOOL )",
        },
        {
            "(flag >= (((b) + 3) * 4))", 2,   // >= at index 2
            "( BOOL )",
        },
        {
            "((flag) + 5 * 3 >= (((b) + 3) * 4))", 8,   // >= at index 8
            "( BOOL )",
        },
    };

    for (int ii = 0; ii < tests.size(); ii++) {
        std::string input = std::get<0>(tests[ii]);
        int index = std::get<1>(tests[ii]);
        std::string expected = std::get<2>(tests[ii]);


        //  generate token* vector from string
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        if (tok_ptrs.back()->getType() == sp::Token::TokenType::EOFF) { tok_ptrs.pop_back(); }
        //

        std::vector<sp::Token*> out_ptrs;
        try {
            CondExprUtils::ParseRelExpr(tok_ptrs, out_ptrs, index);
            auto result = CondExprUtils::VectorToString(out_ptrs);
            REQUIRE(result == expected);
        }
        catch (sp::UtilsException& ex) {
            INFO(ex.what());
            INFO("UE: Test case: " + input);
            REQUIRE(false);
        }
        catch (sp::ParserException& ex) {
            INFO(ex.what());
            INFO("PE: Test case: " + input);
            REQUIRE(false);
        }
    }
}

TEST_CASE("CondExprUtils - checkSubExpr - pass Test") {

    std::vector<std::vector<sp::Token*>> tests{
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
    };

    for (int ii = 0; ii < tests.size(); ii++) {
        auto input = tests[ii];

        try {
            auto result = CondExprUtils::CheckSubExpr(input);
            REQUIRE(result);
        }
        catch (sp::UtilsException& ex) {
            INFO(ex.what());
            //INFO("UE: Test case: " + input);
            INFO("UE: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
        catch (sp::ParserException& ex) {
            INFO(ex.what());
            //INFO("PE: Test case: " + input);
            INFO("PE: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
    }
}

TEST_CASE("CondExprUtils - checkSubExpr - fail Test") {

    std::vector<std::vector<sp::Token*>> tests{
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
        {
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::LPAREN, "("),
            new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
            new sp::Token(sp::Token::TokenType::RPAREN, ")"),
        },
    };

    for (int ii = 0; ii < tests.size(); ii++) {
        auto input = tests[ii];

        try {
            auto result = CondExprUtils::CheckSubExpr(input);
            INFO("Expect Exception: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
        catch (sp::UtilsException& ex) {
            INFO(ex.what());
            REQUIRE(true);
        }
        catch (sp::ParserException& ex) {
            INFO(ex.what());
            INFO("PE: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
    }
}

TEST_CASE("CondExprUtils - checkLeft - pass Test") {

    std::vector<std::tuple<std::vector<sp::Token*>, int, int>> tests{
        {
            {
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::AND, "&&"),
            }, 2, 0 // ) left of && is in index 2, respective ( is at 0th
        },
        {
            {
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::OR, "||"),
            }, 6, 0
        },
        {
            {
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::LPAREN, "("),
                new sp::Token(sp::Token::TokenType::BOOL, "BOOL"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::RPAREN, ")"),
                new sp::Token(sp::Token::TokenType::AND, "&&"),
            }, 9, 3
        },
    };

    for (int ii = 0; ii < tests.size(); ii++) {
        auto input = std::get<0>(tests[ii]);
        auto start = std::get<1>(tests[ii]);
        auto expect = std::get<2>(tests[ii]);

        try {
            auto result = CondExprUtils::SeekParenLeftCheck(input, start);
            REQUIRE(result == expect);
        }
        catch (sp::UtilsException& ex) {
            INFO(ex.what());
            //INFO("UE: Test case: " + input);
            INFO("UE: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
        catch (sp::ParserException& ex) {
            INFO(ex.what());
            //INFO("PE: Test case: " + input);
            INFO("PE: Test Num: " + std::to_string(ii));
            REQUIRE(false);
        }
    }
}
