#pragma once

#include "SP/Token.h"
#include "AST/Index.h"

using namespace std;

//class Lexer {};

//class LexerStub : public Lexer {

class LexerStub { //: public Lexer {
	const std::vector<sp::Token*> tokens;
	int index = 0;
public:
	LexerStub(std::vector<sp::Token*> tokens) :tokens{ tokens } {};
	sp::Token* nextToken();
};

class Parser {
	LexerStub* l_ptr;
	sp::Token* currToken = nullptr; // "curr_peek"
	sp::Token* peekToken = nullptr; //"init_peek";
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
	ast::ReadStmt* parseReadStmt();
	ast::PrintStmt* parsePrintStmt();

	inline sp::Token* getCurrToken() { return currToken; };
	static bool isKeyword(sp::Token* tok);

private:
	bool currTokenIs(sp::Token::TokenType tok_type);
	bool expectPeek(sp::Token::TokenType tok_type);
	bool peekTokenIs(sp::Token::TokenType tok_type);

	bool currTokenIsNameOrKeyword();
	bool expectPeekIsNameOrKeyword();
	bool peekTokenIsNameOrKeyword();

	int getPlusPC();
	std::string genError(std::string str);
	std::string currLiteral();
	std::string peekLiteral();
	bool parseTest();
};
