#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

// test for some bracketings assign expr
TEST_CASE("ParseLexer If - Test") {
    //STRING input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<STRING, STRING>> tests{
            {
                    "if (flag == 1) then { read r; } else { read r; }",
                    "if (( flag == 1 )) then {\n    read (r);\n}\nelse {\n    read (r);\n}\n",
            },
            {
                    "if ((flag) == 1) then { read r; } else {read r;}",
                    "if (( ( flag ) == 1 )) then {\n    read (r);\n}\nelse {\n    read (r);\n}\n",
            },
            {
                    "if ((flag == (1 * 5))|| (3 > 2)) then { x = (2 + 1); } else { print call; }\n",
                    "if (( ( flag == ( 1 * 5 ) ) || ( 3 > 2 ) )) then {\n    (x) = ((2) + (1));\n}\nelse {\n    print (call);\n}\n",
            },
            {
                    "if (flag == 1) then {while ((a + b) > c) { bb = dd;}} else { read r; }",
                    "if (( flag == 1 )) then {\n    while (( ( a + b ) > c )) {\n    (bb) = (dd);\n}\n\n}\nelse {\n    read (r);\n}\n",
            },
    };

    for (int i = 0; i < tests.size(); ++i) {
        STRING input = std::get<0>(tests[i]);
        STRING expected = std::get<1>(tests[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Stmt* is = p.parseStmt();
            REQUIRE(is->toString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}

TEST_CASE("ParseLexer If - Exceptions, Test") {
    //STRING input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<STRING, STRING>> tests{
            {  
                    "if (a + 5) then { read r; } else { read r; }",
                    "if (a + 5) then { read r; } else { read r; }",
            },
            { // relying on others like IF/WHILE to catch this
                    "if (a == b) && (c < 5)) then { read r; } else { read r;}",
                    "if (a == b) && (c < 5)) then { read r;} else { read r;}",
            },
            {
                    "if (a == b) && c < 5) then { read r; } else { read r; }",
                    "if (a == b) && c < 5) then { read r; } else { read r; }",
            },
            {
                    "if ((a == b) && c < 5)) then { read r; } else { read r; }",
                    "if ((a == b) && c < 5)) then { read r; } else { read r; }",
            },
    };

    for (int i = 0; i < tests.size(); ++i) {
        STRING input = std::get<0>(tests[i]);
        STRING expected = std::get<1>(tests[i]);

        /** begin ritual to Summon Parser **/
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        /** Parser now ready for use      **/

        try {
            ast::Stmt* ce = p.parseStmt();
            INFO("Exception Expected: TestCase: " + input + ", got: " + ce->toString());
            REQUIRE(false);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            REQUIRE(true);
        }
    }
}
