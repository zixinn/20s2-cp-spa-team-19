#pragma once
//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"
//#include "SP/CondExprUtils.h"

#include "catch.hpp"
using namespace std;

// NOTE: basic assumption for parseCondExpr is that input ALWAYS has an outermost () 
//       to simulate the condexpr of if and while
TEST_CASE("ParseLexer CondExpr - Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::tuple<std::string, std::string, std::unordered_set<std::string>, std::unordered_set<std::string>>> tests{
        {
            "(!(4 == 5))",
            "( ! ( 4 == 5 ) )",
            {
            },
            {
                "4", "5",
            },
        },
        {
            "(a + 1 > c % 2)",
            "( a + 1 > c % 2 )",
            {
                "a", "c",
            },
            {
                "1", "2",
            },
        },
        {
            "((a + 1 > c % 2) || (a != b))",
            "( ( a + 1 > c % 2 ) || ( a != b ) )",
            {
                "a", "c", "b",
            },
            {
                "1", "2",
            },
        },
    };

    for (int i = 0; i < tests.size(); ++i) {
        std::string input = std::get<0>(tests[i]);
        std::string expected = std::get<1>(tests[i]);
        auto vn_set = std::get<2>(tests[i]);
        auto cv_set = std::get<3>(tests[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            auto ceb = (ast::CondExprBag*)ce;
            REQUIRE(ce->toString() == expected);
            INFO("TestCase: " + input);
            auto vns = ceb->getVarNames();
            REQUIRE(vns.size() == vn_set.size());
            for (std::string vn : vns) {
                INFO("TestCase: " + input + ", VarName not found, val: " + vn);
                REQUIRE(vn_set.count(vn) == 1);
            }
            auto cvs = ceb->getConstVal();
            REQUIRE(cvs.size() == cv_set.size());
            for (std::string cv : cvs) {
                INFO("TestCase: " + input + ", ConstVal not found, val: " + cv);
                REQUIRE(cv_set.count(cv) == 1);
            }
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
        catch (sp::UtilsException &ex) {
            INFO(ex.what());
            INFO("UE :: TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}

TEST_CASE("ParseLexer CondExpr - Keywords Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::tuple<std::string, std::unordered_set<std::string>, std::unordered_set<std::string>>> tests{
        {
            "(!(4 == call))",
            {
                "call",
            },
            {
                "4",
            },
        },
        {
            "(procedure == procedure)",
            {
                "procedure"
            },
            {
            },
        },
        {
            "(1!=print)",
            {
                "print"
            },
            {
                "1",
            },
        },
        {
            "((!(read == 5)) && (a > (if)))",
            {
                "read", "if", "a"
            },
            {
                "5",
            },
        },
        {
            "((!(a != b)) && (else == (((1 + then) / 3) * 5)))",
            {
                "a", "b", "else", "then",
            },
            {
                "5", "1", "3",
            },
        },
        {
            "((!(a + 1 > c % 2)) || (a != b))",
            {
                "a", "b", "c",
            },
            {
                "1", "2",
            },
        },
        {
            "((1) == b)",
            {
                "b",
            },
            {
                "1",
            },
        },
        {
            "((((1))) == b)",
            {
                "b",
            },
            {
                "1",
            },
        },
        
    };

    for (int i = 0; i < tests.size(); ++i) {
        std::string input = std::get<0>(tests[i]);
        auto vn_set = std::get<1>(tests[i]);
        auto cv_set = std::get<2>(tests[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            auto ceb = (ast::CondExprBag*)ce;
            INFO("TestCase: " + input + ", bag: " + ceb->toString());
            auto vns = ceb->getVarNames();
            REQUIRE(vns.size() == vn_set.size());
            for (std::string vn : vns) {
                INFO("TestCase: " + input + ", VarName not found, val: " + vn);
                REQUIRE(vn_set.count(vn) == 1);
            }
            auto cvs = ceb->getConstVal();
            REQUIRE(cvs.size() == cv_set.size());
            for (std::string cv : cvs) {
                INFO("TestCase: " + input + ", ConstVal not found, val: " + cv);
                REQUIRE(cv_set.count(cv) == 1);
            }
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
        catch (sp::UtilsException &ex) {
            INFO(ex.what());
            INFO("UE :: TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}

// These were originally from parseLexer While
TEST_CASE("ParseLexer CondExpr - Exceptions Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::string> tests{
            "(a == b) && c < 5",
            "1 == b;",
            "!1 == b",
            "a == b && c < 5",
            "((a)) == b && c < 5",
            "a == b && (c < 5)",
            // the following will cause an exception in parseIF or parseWHILE
            //"a == b) && (c < 5)",
            // need parenthesis around !, for the ||
            "!(a + 1 > c % 2) || (a != b)",
            "(!(a + 1 > c % 2) || (a != b)",
    };

    for (int i = 0; i < tests.size(); ++i) {
        // this bit is to simulate the if ( ... )
        std::string input = "(" + tests[i] + ")";

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            auto ceb = (ast::CondExprBag*)ce;
            INFO("TestCase: " + input + ", bag: " + ceb->toString());
            REQUIRE(false);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(true);
        }
        catch (sp::UtilsException &ex) {
            INFO(ex.what());
            INFO("UE :: TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}

