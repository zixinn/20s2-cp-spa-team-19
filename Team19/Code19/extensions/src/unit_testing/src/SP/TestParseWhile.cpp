#include "AST/Index.h"
#include "SP/Parser.h"
#include "SP/ParserUtils.h"

#include "catch.hpp"
using namespace std;

// test for some bracketings assign expr
TEST_CASE("ParseLexer While - Test") {

    //STRING input = "if = v + x * y + z * t;";    // if not implemented yet
    std::vector<std::pair<STRING, STRING>> tests{
            {
                    "while (flag == 1) { read r;}",
                    "while (( flag == 1 )) {\n    read (r);\n}\n",
            },
            {
                    "while ((flag == 1) || (5 > 3)) { read r; }",
                    "while (( ( flag == 1 ) || ( 5 > 3 ) )) {\n    read (r);\n}\n",
            },
            {
                    "while ((flag == 1) || (5 > 3)) { asd = 23; b = 2 + sdf; }",
                    "while (( ( flag == 1 ) || ( 5 > 3 ) )) {\n    (asd) = (23);\n    (b) = ((2) + (sdf));\n}\n",
            },
            {
                    "while ((flag == 1) || (5 > 3)) { asd = 23; b = 2 + sdf; while (a + 1 > c % 2) { sss = 123 + 22; call beef; print while; call beef; }}",
                    "while (( ( flag == 1 ) || ( 5 > 3 ) )) {\n    (asd) = (23);\n    (b) = ((2) + (sdf));\n    while (( a + 1 > c % 2 )) {\n    (sss) = ((123) + (22));\n    call beef;\n    print (while);\n    call beef;\n}\n\n}\n",
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
            ast::WhileStmt* ws = p.parseWhileStmt();
            REQUIRE(ws->toString() == expected);
        }
        catch (sp::ParserException &ex) {
            INFO(ex.what());
            INFO("TestCase: " + input + ", Exception Thrown");
            REQUIRE(false);
        }
    }
}


