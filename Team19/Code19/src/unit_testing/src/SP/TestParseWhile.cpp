//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

//// test for some bracketings assign expr
//TEST_CASE("ParseLexer While - Test") {
//
//    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
//    std::vector<std::pair<std::string, std::string>> tests{
//        {
//            "while (flag == 1) {}",
//            "while (flag == 1) {\n}\n",
//        },
//        {
//            "while ((flag == 1) || (5 > 3)) {}",
//            "while ((flag == 1) || (5 > 3)) {\n}\n",
//        },
//        {
//            "while ((flag == 1) || (5 > 3)) { asd = 23; b = 2 + sdf; }",
//            "while ((flag == 1) || (5 > 3)) {\n    asd = 23;\n    b = (2 + sdf);\n}\n",
//        },
//        {
//            "while ((flag == 1) || (5 > 3)) { asd = 23; b = 2 + sdf; while (a + 1 > c % 2) { sss = 123 + 22; call beef; print while; call beef; }}",
//            "while ((flag == 1) || (5 > 3)) {\n    asd = 23;\n    b = (2 + sdf);\n    while ((a + 1) > (c % 2)) {\n    sss = (123 + 22);\n    call beef;\n    print while;\n    call beef;\n}\n\n}\n",
//        },
//    };
//
//    for (int i = 0; i < tests.size(); ++i) {
//        std::string input = std::get<0>(tests[i]);
//        std::string expected = std::get<1>(tests[i]);
//
//        /** begin ritual to Summon Parser **/
//        std::vector<sp::Token> actual_tok;
//        std::vector<sp::Token*> tok_ptrs;
//        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
//        auto l = new LexerStub(tok_ptrs);
//        auto p = Parser(l);
//        /** Parser now ready for use      **/
//
//        try {
//            ast::WhileStmt* ws = p.parseWhileStmt();
//            REQUIRE(ws->toString() == expected);
//        }
//        catch (sp::ParserException &ex) {
//            INFO(ex.what());
//            INFO("TestCase: " + input + ", Exception Thrown");
//            REQUIRE(false);
//        }
//    }
//}
//
//
////TEST_CASE("ParseLexer While - Keywords, Test") {
////
////    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
////    std::vector<std::pair<std::string, std::string>> tests{
////        {
////            "!(4 == call)",
////            "!(4 == call)",
////        },
////        {
////            "procedure == procedure;",
////            "procedure == procedure",
////        },
////        {
////            "1!=print;",
////            "1 != print",
////        },
////        {
////            "(!(read == 5)) && (a > (if))",
////            "(!(read == 5)) && (a > if)",
////        },
////        {
////            "(!(a != b)) && (else == (1+then) / 3 * (5))",
////            "(!(a != b)) && (else == (((1 + then) / 3) * 5))",
////        },
////        {
////            "(1) == b;",
////            "1 == b",
////        },
////        {
////            "(((1))) == b;",
////            "1 == b",
////        },
////    };
////
////    for (int i = 0; i < tests.size(); ++i) {
////        std::string input = std::get<0>(tests[i]);
////        std::string expected = std::get<1>(tests[i]);
////
////        /** begin ritual to Summon Parser **/
////        std::vector<sp::Token> actual_tok;
////        std::vector<sp::Token*> tok_ptrs;
////        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
////        auto l = new LexerStub(tok_ptrs);
////        auto p = Parser(l);
////        /** Parser now ready for use      **/
////
////        try {
////            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
////            REQUIRE(ce->toString() == expected);
////        }
////        catch (sp::ParserException &ex) {
////            INFO(ex.what());
////            INFO("TestCase: " + input + ", Exception Thrown");
////            REQUIRE(false);
////        }
////    }
////}
////
////TEST_CASE("ParseLexer While - Exceptions, Test") {
////    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
////    std::vector<std::pair<std::string, std::string>> tests{
////        {
////            "!1 == b;",
////            "1 == b",
////        },
////        {
////            "a == b && c < 5;",
////            "(a == b) && (c < 5)",
////        },
////        {
////            "((a)) == b && c < 5;",
////            "(a == b) && (c < 5)",
////        },
////        {
////            "a == b && (c < 5);",
////            "(a == b) && (c < 5)",
////        },
////        //{ // relying on others like IF/WHILE to catch this
////        //    "a == b) && (c < 5);",
////        //    "(a == b) && (c < 5)",
////        //},
////        {
////            "(a == b) && c < 5;",
////            "(a == b) && (c < 5)",
////        },
////    };
////
////    for (int i = 0; i < tests.size(); ++i) {
////        std::string input = std::get<0>(tests[i]);
////        std::string expected = std::get<1>(tests[i]);
////
////        /** begin ritual to Summon Parser **/
////        std::vector<sp::Token> actual_tok;
////        std::vector<sp::Token*> tok_ptrs;
////        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
////        auto l = new LexerStub(tok_ptrs);
////        auto p = Parser(l);
////        /** Parser now ready for use      **/
////
////        try {
////            ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
////            //REQUIRE(ce->toString() == expected);
////            INFO("Exception Expected: TestCase: " + input + ", got: " + ce->toString());
////            REQUIRE(false);
////        }
////        catch (sp::ParserException &ex) {
////            INFO(ex.what());
////            //INFO("TestCase: " + input + ", Exception Thrown");
////            REQUIRE(true);
////        }
////    }
////}
