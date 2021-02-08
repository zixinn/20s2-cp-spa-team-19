//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

// Prototype test to ensure mechanism is working
TEST_CASE("Parse LexerStubAdapt Test") {
    std::string input = "nobody = 10;";

    /** begin ritual to Summon Parser **/
	vector<sp::Token> actualTokens;
	REQUIRE(Lexer::tokenise(input, actualTokens));
    // converts sp::Token to sp::Token*
	std::vector<sp::Token*> tok_ptrs;
    ParserUtils::LexerStubAdapt(actualTokens, tok_ptrs);
	REQUIRE(tok_ptrs.size() == actualTokens.size());
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);
    /** Parser now ready for use      **/

    ast::AssignStmt* ass = p.parseAssignStmt();
    REQUIRE(ass->toString() == input);
    //REQUIRE(false);
    //for (int i = 0; i < tok_ptrs.size(); ++i) {
    //    std::cout << (*tok_ptrs[i]).getLiteral() << std::endl;
    //}
}

// Duplicate this Test 
TEST_CASE("ParseLexer Stmt Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::string input = "x = v + x * y + z * t;";
    std::string expected = "x = ((v + (x * y)) + (z * t));";

    /** begin ritual to Summon Parser **/
	std::vector<sp::Token> actual_tok;
	std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);
    /** Parser now ready for use      **/

    ast::Stmt* stmt = p.parseStmt();
    REQUIRE(stmt->toString() == expected);
}


// test for some bracketings assign expr
TEST_CASE("ParseLexer Assign Paren - Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "xx = 1 + (2 + 3) + 4;",
            "xx = ((1 + (2 + 3)) + 4);",
        },
        {
            "yy = 2/(5+    5);",
            "yy = (2 / (5 + 5));",
        },
        {
            "zz = 1;",
            "zz = 1;",
        },
        {
            "aa = (1+(2));",
            "aa = (1 + 2);",
        },
        {
            "bb = (b);",
            "bb = b;",
        },
        {
            "cc = (1) + ((2) + (3)) + (4);",
            "cc = ((1 + (2 + 3)) + 4);",
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
            ast::Stmt* stmt = p.parseStmt();
            REQUIRE(stmt->toString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            REQUIRE(false);
        }
    }
}


// test for keywords on LHS of assign
TEST_CASE("ParseLexer Assign Keyword LHS - Test") {

    // if and while not ready yet
    // also dont forget to test for  THEN, ELSE
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "call = 1+2+3+4;",
            "call = (((1 + 2) + 3) + 4);"
        },
        {
            "read = 2/5*3/6;",
            "read = (((2 / 5) * 3) / 6);"
        },
        {
            "print = a * b / ( c - a ) / b / c - z;",
            "print = (((((a * b) / (c - a)) / b) / c) - z);"
        },
        {
            "procedure=a;",
            "procedure = a;",
        },
        {
            "program=(1);",
            "program = 1;",
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

        ast::Stmt* stmt = p.parseStmt();
        REQUIRE(stmt->toString() == expected);
    }
}
