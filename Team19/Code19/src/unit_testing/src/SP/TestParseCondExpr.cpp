//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

// test for some bracketings assign expr
// NOTE: for some reason, Lexer::Tokenize expects a RPAREN ) or SEMICOLON ; to terminate string
TEST_CASE("ParseLexer CondExpr - Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "!(4 == 5);",
            "!(4 == 5)",
        },
        {
            "1 == b;",
            "1 == b",
        },
        {
            "a + 1 > c % 2",
            "(a + 1) > (c % 2)",
        },
        {
            "(a + 1 > c % 2) || (a != b)",
            "((a + 1) > (c % 2)) || (a != b)",
        },
        {
            "(!(4 == 5)) && (a > (b))",
            "(!(4 == 5)) && (a > b)",
        },
        {
            "((((!(4 == 5))))) && (a > (b))",
            "(!(4 == 5)) && (a > b)",
        },
        {
            "(((((!(4 == 5))))) && (a > (b)))",
            "(!(4 == 5)) && (a > b)",
        },
        {
            "(!(a != b)) && (d == (1+j) / 3 * (5))",
            "(!(a != b)) && (d == (((1 + j) / 3) * 5))",
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
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            REQUIRE(ce->toString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}


TEST_CASE("ParseLexer CondExpr - Keywords, Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "!(4 == call)",
            "!(4 == call)",
        },
        {
            "procedure == procedure;",
            "procedure == procedure",
        },
        {
            "1!=print;",
            "1 != print",
        },
        {
            "(!(read == 5)) && (a > (if))",
            "(!(read == 5)) && (a > if)",
        },
        {
            "(!(a != b)) && (else == (1+then) / 3 * (5))",
            "(!(a != b)) && (else == (((1 + then) / 3) * 5))",
        },
        {
            "(1) == b;",
            "1 == b",
        },
        {
            "(((1))) == b;",
            "1 == b",
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
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            REQUIRE(ce->toString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}

TEST_CASE("ParseLexer CondExpr - Exceptions, Test") {
    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "!1 == b;",
            "1 == b",
        },
        {
            "a == b && c < 5;",
            "(a == b) && (c < 5)",
        },
        {
            "((a)) == b && c < 5;",
            "(a == b) && (c < 5)",
        },
        {
            "a == b && (c < 5);",
            "(a == b) && (c < 5)",
        },
        //{ // relying on others like IF/WHILE to catch this
        //    "a == b) && (c < 5);",
        //    "(a == b) && (c < 5)",
        //},
        {   // need parenthesis around !, for the ||
            "!(a + 1 > c % 2) || (a != b)",
            "!(a + 1 > c % 2) || (a != b)",
        },
        {   
            "(!(a + 1 > c % 2)) || (a != b)",
            "(!(a + 1 > c % 2)) || (a != b)",
        },
        {
            "(a == b) && c < 5;",
            "(a == b) && (c < 5)",
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
            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
            //REQUIRE(ce->toString() == expected);
            INFO("Exception Expected: TestCase: " + input + ", got: " + ce->toString());
            REQUIRE(false);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            //INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(true);
        }
    }
}
