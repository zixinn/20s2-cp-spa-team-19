//#include "Parser.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"
#include "PKB/PKB.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("ParserUtils - getCallsInProc Test") {

    std::vector<std::pair<std::string, std::unordered_set<std::string>>> ts{
        {
            "call me;",
            { "me" },
        },
        {
            "x = 3 + 1; call me; call abc; read story; call help;",
            { "me", "abc", "help" },
        },
        {
            "call me; while (asd > asd) { call abc; } call help;",
            { "me", "abc", "help" },
        },
        {
            "call me; if (asd > asd) then { call abc; } else { call def; } call help;",
            { "me", "abc", "def", "help" },
        },
    };

    for (int i = 0; i < ts.size(); ++i) {
        std::string test = std::get<0>(ts[i]);
        std::unordered_set<std::string> ss = std::get<1>(ts[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(test, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::StmtLst* sl = p.parseStmtLst();
            std::vector<std::string> out;
            ParserUtils::getCallsInProc(sl->getStatements(), out);

            INFO("Stmt in StmtLst : " + std::to_string(sl->getStatements().size()));
            REQUIRE(out.size() == ss.size());

            for (int ii = 0; ii < out.size(); ++ii) {
                bool is_in = ss.find(out[ii]) != ss.end();
                INFO("This is not in set : " + out[ii]);
                REQUIRE(is_in);
            }
        }
        catch (sp::ParserException& ex) {
            INFO(ex.what());
            REQUIRE(false);
        }
    }
}

TEST_CASE("ParserUtils - isCyclic Test") {

    std::vector<std::tuple<std::string, bool, std::unordered_set<std::string>>> ts{
        {
            "procedure abc { x = x + 1; }",
            false,
            {},
        },
        {
            "procedure abc { call abc; }",
            true,
            {
                "abc",
            },
        },
        {
            "procedure abc { call xyz; call def; } procedure def { call abc; } procedure xyz { x = x + 1; }",
            true,
            {
                "abc", "def",
            },
        },
        //{   // need another function to check if calling non-existent function
        //    // dont combine with isCyclic
        //    "procedure abc { call def; }",
        //    true,
        //},
        {   // need to unset visited after returning from it?
            // https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
            "procedure abc { x = x + 1; } procedure def { call abc; call ghi; } procedure ghi { call abc; }",
            false,
            {},
        },
        {
            "procedure abc { call qrs; call def; } procedure def { call qrs; call hij; } procedure hij { call klm; } procedure klm { call nop; } procedure nop { call def; } procedure qrs { x = x + 1; }",
            true,
            { "abc", "def", "hij", "klm", "nop" },
        },
        {   // testing if
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { x = x + 1; } else { call ghi; } } procedure ghi { call def; }",
            true,
            { "abc", "def", "ghi" },
        },
        {   
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call jkl; } } procedure ghi { call def; } procedure jkl { x = x + 1; }",
            true,
            { "abc", "def", "ghi" },
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; } } procedure ghi { call jkl; } procedure jkl { call def; }",
            true,
            { "abc", "def", "ghi", "jkl" },
        },
    };

    // need to redo the cyclic detection to see what we are testing
    for (int i = 0; i < ts.size(); ++i) {
        std::string test = std::get<0>(ts[i]);
        bool is_cyclic = std::get<1>(ts[i]);
        std::unordered_set<std::string> ss = std::get<2>(ts[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(test, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Program* prog = p.parseProgram();
            std::unordered_set<std::string> out;
            bool result = ParserUtils::isASTCyclic(prog, out);
            INFO("Test " + std::to_string(i) + ", Output Path was: " + ParserUtils::setToString(out));
            REQUIRE(result == is_cyclic);

            // if test is not cyclic dont need to print path
            if (!is_cyclic) { continue; }

            // test is not cyclic check if path matches
            // https://en.cppreference.com/w/cpp/container/unordered_set/operator_cmp
            INFO("Test " + std::to_string(i) + ", Output Path was: " + ParserUtils::setToString(out));
            REQUIRE(ss == out);
        }
        catch (sp::ParserException& ex) {
            INFO("Test " + std::to_string(i) + " : " + ex.what());
            REQUIRE(false);
        }
    }
}

TEST_CASE("ParserUtils - hasUndefCall Test") {

    std::vector<std::tuple<std::string, bool, std::unordered_set<std::string>>> ts{
        {
            "procedure abc { x = x + 1; }",
            false,
            {},
        },
        {
            "procedure abc { x = x + 1; call def; }",
            true,
            { "def", },
        },
        {   
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call jkl; } } procedure ghi { call def; } procedure jkl { x = x + 1; }",
            false,
            {},
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; } } procedure ghi { call jkl; } procedure jkl { call def; }",
            false,
            {},
        },
        {   // testing if
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call xyz; } } procedure jkl { x = x + 1; }",
            true,
            { "ghi", "xyz" },
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; call jkl; call mno; } } ",
            true,
            { "ghi", "jkl", "mno" },
        },
    };

    for (int i = 0; i < ts.size(); ++i) {
        std::string test = std::get<0>(ts[i]);
        bool has_undef_call = std::get<1>(ts[i]);
        std::unordered_set<std::string> ss = std::get<2>(ts[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(test, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Program* prog = p.parseProgram();
            std::unordered_set<std::string> out;
            bool result = ParserUtils::hasUndefCall(prog, out);
            INFO("Test " + std::to_string(i) + ", Output Path was: " + ParserUtils::setToString(out));
            REQUIRE(result == has_undef_call);

            // if test does not call undefined procedure, there is nothing to check
            if (!has_undef_call) { continue; }

            // checks if undef call detected is same as what is expected
            INFO("Test " + std::to_string(i) + ", Output Path was: " + ParserUtils::setToString(out));
            REQUIRE(ss == out);
        }
        catch (sp::ParserException& ex) {
            INFO("Test " + std::to_string(i) + " : " + ex.what());
            REQUIRE(false);
        }
    }
}


TEST_CASE("ParserUtils - throwIfCyclic Test") {

    std::vector<std::tuple<std::string, bool, std::unordered_set<std::string>>> ts{
        {
            "procedure abc { x = x + 1; }",
            false,
            {},
        },
        {
            "procedure abc { call abc; }",
            true,
            {
                "abc",
            },
        },
        {
            "procedure abc { call xyz; call def; } procedure def { call abc; } procedure xyz { x = x + 1; }",
            true,
            {
                "abc", "def",
            },
        },
        {   // need to unset visited after returning from it?
            // https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
            "procedure abc { x = x + 1; } procedure def { call abc; call ghi; } procedure ghi { call abc; }",
            false,
            {},
        },
        {
            "procedure abc { call qrs; call def; } procedure def { call qrs; call hij; } procedure hij { call klm; } procedure klm { call nop; } procedure nop { call def; } procedure qrs { x = x + 1; }",
            true,
            { "abc", "def", "hij", "klm", "nop" },
        },
        {   // testing if
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { x = x + 1; } else { call ghi; } } procedure ghi { call def; }",
            true,
            { "abc", "def", "ghi" },
        },
        {   
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call jkl; } } procedure ghi { call def; } procedure jkl { x = x + 1; }",
            true,
            { "abc", "def", "ghi" },
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; } } procedure ghi { call jkl; } procedure jkl { call def; }",
            true,
            { "abc", "def", "ghi", "jkl" },
        },
    };

    // need to redo the cyclic detection to see what we are testing
    for (int i = 0; i < ts.size(); ++i) {
        std::string test = std::get<0>(ts[i]);
        bool is_cyclic = std::get<1>(ts[i]);
        std::unordered_set<std::string> ss = std::get<2>(ts[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(test, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Program* prog = p.parseProgram();

            ParserUtils::throwIfCyclic(prog);

            if (is_cyclic) {
                // supposed to throw an exception
                REQUIRE(false);
            }
            else {
                REQUIRE(true);
            }
        }
        catch (sp::UtilsException& ex) {
            INFO("Test " + std::to_string(i) + " : " + ex.what());
            REQUIRE(true);
        }
    }
}

TEST_CASE("ParserUtils - throwIfUndef Test") {

    std::vector<std::tuple<std::string, bool, std::unordered_set<std::string>>> ts{
        {
            "procedure abc { x = x + 1; }",
            false,
            {},
        },
        {
            "procedure abc { x = x + 1; call def; }",
            true,
            { "def", },
        },
        {   
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call jkl; } } procedure ghi { call def; } procedure jkl { x = x + 1; }",
            false,
            {},
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; } } procedure ghi { call jkl; } procedure jkl { call def; }",
            false,
            {},
        },
        {   // testing if
            "procedure abc { x = x + 1; call def; } procedure def { if (b > 3) then { call ghi; } else { call xyz; } } procedure jkl { x = x + 1; }",
            true,
            { "ghi", "xyz" },
        },
        {   // testing while
            "procedure abc { x = x + 1; call def; } procedure def { while (b > 3) { x = x + 1; call ghi; call jkl; call mno; } } ",
            true,
            { "ghi", "jkl", "mno" },
        },
    };

    for (int i = 0; i < ts.size(); ++i) {
        std::string test = std::get<0>(ts[i]);
        bool has_undef_call = std::get<1>(ts[i]);
        std::unordered_set<std::string> ss = std::get<2>(ts[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(test, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Program* prog = p.parseProgram();
            ParserUtils::throwIfUndefCall(prog);

            if (has_undef_call) {
                // supposed to throw an exception
                REQUIRE(false);
            }
            else {
                REQUIRE(true);
            }
        }
        catch (sp::UtilsException& ex) {
            INFO("Test " + std::to_string(i) + " : " + ex.what());
            REQUIRE(true);
        }
    }
}


