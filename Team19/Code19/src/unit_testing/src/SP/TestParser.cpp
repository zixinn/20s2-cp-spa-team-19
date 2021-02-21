//#include "Parser.h"
#include <string>
#include <vector>
#include <iostream>
#include "AST/Index.h"
#include "SP/Parser.h"
#include "PKB/PKB.h"

#include "catch.hpp"
using namespace std;

const std::string TOKEN_LIT_ASSIGN = "assign";

/*
void require(bool b) {
    REQUIRE(b);
}
*/

TEST_CASE("Initial Node Test") {

    sp::Token::TokenType tok_type = sp::Token::TokenType::PLUS;
	std::string tok_lit = "plus";
    sp::Token tok = sp::Token(tok_type, tok_lit);

    ast::Node * node_ptr = new ast::StubNode(new sp::Token(tok_type, tok_lit));
	//ast::Node* node_ptr = ast::Node::makeStub(tokenLiteral);
	
	//auto flag = tok_lit.compare((*node_ptr).getTokenLiteral());
    REQUIRE(tok_lit == (*node_ptr).getTokenLiteral());
	//REQUIRE(node_ptr->token->compare(&tok));
	//auto flag = tokenLiteral.compare((*node_ptr).getTokenLiteral());
	//REQUIRE(flag == 0);
}

TEST_CASE("Parser NextToken Test") {

    std::vector<sp::Token*> stubTokens{ 
        new sp::Token(sp::Token::TokenType::PROC, "procedure"),
        new sp::Token(sp::Token::TokenType::NAME, "main"),
        new sp::Token(sp::Token::TokenType::LBRACE, "{"),
        new sp::Token(sp::Token::TokenType::NAME, "x"),
        new sp::Token(sp::Token::TokenType::EQ, "="),
        new sp::Token(sp::Token::TokenType::CONST, "10"),
        new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
        new sp::Token(sp::Token::TokenType::RBRACE, "}"),
        new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };
    std::vector<string> stubTokensOld{ "procedure", "main", "{", "x", "=", "10", ";", "}", "EOF" };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);

    REQUIRE(p.getCurrToken()->getLiteral() == "procedure");
    REQUIRE(p.getCurrToken()->compare(stubTokens[0]));

    for (sp::Token* token : stubTokens) {
        auto tmp_tok = p.getCurrToken();
	    REQUIRE(token->compare(tmp_tok));
        p.nextToken();
    }
    std::cout << p.getCurrToken()->getLiteral() << std::endl;
}


TEST_CASE("Parse VarName Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "foobar"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::VarName* vn = p.parseVarName();
    REQUIRE(p.getCurrToken()->compare(stubTokens[0]));
    REQUIRE(vn->getToken()->compare(stubTokens[0]));
    REQUIRE(vn->val == stubTokens[0]->getLiteral());
    
}

TEST_CASE("Parse ConstVal Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::CONST, "55"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::ConstVal* cv = p.parseConstVal();
    REQUIRE(cv->getToken()->compare(stubTokens[0]));
    REQUIRE(cv->getVal() == 55);
}

TEST_CASE("Parse ProcName Test") {
    std::vector<sp::Token*> stubTokens{
        new sp::Token(sp::Token::TokenType::NAME, "procedure"),
    };

    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::ProcName* pn = p.parseProcName();
    REQUIRE(p.getCurrToken()->compare(stubTokens[0]));
    REQUIRE(pn->getToken()->compare(stubTokens[0]));
    REQUIRE(pn->val == stubTokens[0]->getLiteral());
    
}

// Test Keyword Detection
TEST_CASE("Keyword Detection Test") {
    std::vector<std::tuple<sp::Token*, bool>> tok_tests {
        { new sp::Token(sp::Token::TokenType::NAME, "sun"), false},
        { new sp::Token(sp::Token::TokenType::SEMICOLON, ";"), false },
        { new sp::Token(sp::Token::TokenType::PROC, "procedure"), true },
        { new sp::Token(sp::Token::TokenType::READ, "read"), true },
        { new sp::Token(sp::Token::TokenType::PRINT, "print"), true },
        { new sp::Token(sp::Token::TokenType::CALL, "call"), true },
        { new sp::Token(sp::Token::TokenType::WHILE, "while"), true },
        { new sp::Token(sp::Token::TokenType::IF, "if"), true },
        { new sp::Token(sp::Token::TokenType::THEN, "then"), true },
        { new sp::Token(sp::Token::TokenType::ELSE, "else"), true },
        { new sp::Token(sp::Token::TokenType::EOFF, "EOF"), false },
    };

    for (int i = 0; i < tok_tests.size(); ++i) {
        auto tok = std::get<0>(tok_tests[i]);
        bool expect = std::get<1>(tok_tests[i]);
        INFO("Token Tested: " + tok->getLiteral());
        REQUIRE(Parser::isKeyword(tok) == expect);
    }
}
