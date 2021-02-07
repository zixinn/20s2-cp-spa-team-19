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
TEST_CASE("ParseLexer CondExpr - Test") {

    //std::string input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<std::string, std::string>> tests{
        {
            "!(4 == 5)",
            "!(4 == 5)",
        },
        {
            "!(a != b)&& (d == (1+j) / 3 * (5))",
            "(!(a != b) && (d == (((1 + j) / 3) * 5)))",
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

        ast::CondExpr* ce = p.parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);
        REQUIRE(ce->toString() == expected);
    }
}


