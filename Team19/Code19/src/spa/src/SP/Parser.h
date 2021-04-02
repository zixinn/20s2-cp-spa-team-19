#pragma once

#include <unordered_map>
#include <unordered_set>
#include "SP/Token.h"
#include "AST/Index.h"
#include "SP/ParserUtils.h"
#include "SP/CondExprUtils.h"
#include "SP/ParserException.h"

using namespace std;

//class Lexer {};

//class LexerStub : public Lexer {

class LexerStub { //: public Lexer {
	const std::vector<sp::Token*> tokens;
	int index = 0;
public:
	LexerStub(std::vector<sp::Token*> tokens) :tokens{ tokens } {};
	//static void LexerStubAdapt(std::vector<sp::Token>& tokens, std::vector<sp::Token*>& out);
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
	ast::ConstVal* parseConstVal();
	ast::StmtLst* parseStmtLst();
	ast::Stmt* parseStmt();
	ast::AssignStmt* parseAssignStmt();
	ast::CallStmt* parseCallStmt();
	ast::ReadStmt* parseReadStmt();
	ast::PrintStmt* parsePrintStmt();
	ast::Proc* parseProc();
	ast::Program* parseProgram();
	ast::Expr* parseExpr(int precedence);
	ast::CondExpr* parseCondExpr(int precedence);
	ast::WhileStmt* parseWhileStmt();
	ast::IfStmt* parseIfStmt();

	//parse the whole thing including calls to DE
	//Return true if no error encountered, else false
	bool parse();

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
	//std::string genError(std::string str);
	sp::ParserException genError(std::string str);
	sp::ParserException genExprError(std::string str);
	sp::ParserException genCondExprError(std::string str);
	std::string currLiteral();
	std::string peekLiteral();
	bool parseTest();

	// expr
	ast::Expr* parsePrefixExpr(sp::Token* tok);
	ast::Expr* parseInfixExpr(ast::Expr*);
	ast::Expr* parseLParenPrefixExpr();

	int peekPrecedence();
	int currPrecedence();

	//calls to DE
	//void addStmtLstToDE(vector<ast::Stmt*> stmts);
	void addStmtLstToDE(vector<ast::Stmt*> stmts, ast::Proc* proc);

};
