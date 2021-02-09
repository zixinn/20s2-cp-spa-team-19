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
TEST_CASE("CondExprUtils2 - Test") {
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

