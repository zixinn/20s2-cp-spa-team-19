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

//shld it be in integration testing?
TEST_CASE("Call DE Test") {
    /*
    procedure star {
     1   x = 1 + 2 * a;
     2   while (v > 1) {
     3       x = 2;
     4       if (v == 1) then {
     5           c = a;
            } else {
     6          b = 2;
            }
        }
     7   print p;
     8   read s;
      }
     */
    string input = "procedure star { x = 1 + 2 * a; while (v > 1) { x = 2; if(v==1)then{c = a;}else{ b = 2;}} print p; read s; } ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse());

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); 
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); 
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == false); 
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); 
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == false);


    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");
    ID varID2 = PKB::varTable->getVarID("c");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "c");
    ID varID3 = PKB::varTable->getVarID("b");
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "b");
    ID varID4 = PKB::varTable->getVarID("p");
    REQUIRE(varID4 == 5);
    REQUIRE(PKB::varTable->getVarName(varID4) == "p");
    ID varID5 = PKB::varTable->getVarID("s");
    REQUIRE(varID5 == 6);
    REQUIRE(PKB::varTable->getVarName(varID5) == "s");

    ID condVarId = PKB::varTable->getVarID("v");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "v");



    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("3") == false);



    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); 
    REQUIRE(PKB::follows->getFollower(2) != 4); 
    REQUIRE(PKB::follows->getFollower(2) != 5);
    REQUIRE(PKB::follows->getFollower(2) == 7); 
    REQUIRE(PKB::follows->getFollower(3) == 4); 
    REQUIRE(PKB::follows->getFollower(4) != 5); 
    REQUIRE(PKB::follows->getFollower(4) != 6); 
    REQUIRE(PKB::follows->getFollower(5) != 6); 
    REQUIRE(PKB::follows->getFollower(7) == 8);
    REQUIRE(PKB::follows->getFollower(8) != 8); // stmt cannot follow itself

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(5) == 4);
    REQUIRE(PKB::parent->getParent(6) == 4);
    REQUIRE(PKB::parent->getParent(7) == -1);

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1, 2, 4, 5 });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ 7 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{  }); 
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, varID1, varID4 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2, 3 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<ID>{ 2, 4, 5 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 4, 6 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID5) == unordered_set<ID>{ 8 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{  });


}

TEST_CASE("Call DE - Exceptions Test") {
    vector<string> inputs{
        //syntax error
        "procedure star { x = 1 + 2 * a; while (v > 1) { x = 2 if(v==1)then{c = a;}else{ b = 2;}} print p; read s; } ",
        //invalid simple
        "abc",
        //invalid expression
        "procedure moon { x = (1+2) ** 3; }",
        //invalid cond expresion
        "procedure moon { if((x+1) > 3 || (a < b)) then {c=a;}else{ b = 2;}}"

    };

    for (int i = 0; i < inputs.size(); i++) {
        string input = inputs[i];
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        REQUIRE_FALSE(p.parse());
    }
}
