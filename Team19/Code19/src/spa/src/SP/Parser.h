#pragma once

#include "ast/Index.fwd.h"
#include "SP/Token.h"

using namespace std;

//class Lexer {};

//class LexerStub : public Lexer {

class LexerStub { //: public Lexer {
	const std::vector<Token*> tokens;
	int index = 0;
public:
	LexerStub(std::vector<Token*> tokens) :tokens{ tokens } {};
	Token* nextToken();
};

class Parser {
	LexerStub* l_ptr;
	Token* currToken = nullptr; // "curr_peek"
	Token* peekToken = nullptr; //"init_peek";
	int pc = 1;
public:
	Parser(LexerStub* l_ptr); 
	void nextToken();
	ast::VarName* parseVarName();
	ast::ProcName* parseProcName();
	ast::StmtLst* parseStmtLst();
	ast::Stmt* parseStmt();
	ast::AssignStmt* parseAssignStmt();
	ast::CallStmt* parseCallStmt();
	//ast::ReadStmt* parseReadStmt();
	//ast::PrintStmt* parsePrintStmt();

	inline Token* getCurrToken() { return currToken; };
	bool currTokenIs(Token::TokenType tok_type);
	bool expectPeek(Token::TokenType tok_type);
	bool peekTokenIs(Token::TokenType tok_type);

	int getPlusPC();
	std::string genError(std::string str);
	std::string currLiteral();
	std::string peekLiteral();
	bool parseTest();
};
