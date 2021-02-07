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
