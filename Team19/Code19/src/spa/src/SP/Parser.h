#pragma once

#include <unordered_map>
#include <unordered_set>
#include "SP/Token.h"
#include "AST/Index.h"

using namespace std;

//class Lexer {};

//class LexerStub : public Lexer {

// function pointers for ExprParsing
namespace ParserUtils {

	// BODMAS precedence, higher the more impt
	namespace ExprPrecedence {		// the order matters
		const int BLANK = 0;
		const int LOWEST = 1;
		const int ADDMINUS = 2;		// operators: + -
		const int DIVMULT = 3;		// operators: * / %
		const int PREFIX = 4;		// operators: !
	};

	// map tokens to correct BODMAS precedence ranking
	const unordered_map<sp::Token::TokenType, int, sp::tokentype_hash> exprRanks{
		{sp::Token::TokenType::PLUS, ExprPrecedence::ADDMINUS },
		{sp::Token::TokenType::MINUS, ExprPrecedence::ADDMINUS },
		{sp::Token::TokenType::DIV, ExprPrecedence::DIVMULT },
		{sp::Token::TokenType::TIMES, ExprPrecedence::DIVMULT },
		{sp::Token::TokenType::MOD, ExprPrecedence::DIVMULT },
	};

	inline const bool hasExprRank(sp::Token::TokenType tok_type) {
		return exprRanks.find(tok_type) != exprRanks.end();
	}
	inline const int getExprRankUnsafe(sp::Token::TokenType tok_type) {
		return exprRanks.at(tok_type);
	}

	// to determine what can pass as a PROCNAME or VARNAME
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> keywords {
		sp::Token::TokenType::PROC,
		sp::Token::TokenType::READ,
		sp::Token::TokenType::PRINT,
		sp::Token::TokenType::CALL,
		sp::Token::TokenType::WHILE,
		sp::Token::TokenType::IF,
		sp::Token::TokenType::THEN,
		sp::Token::TokenType::ELSE,
	};

	inline const bool isKeyword(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return keywords.find(tok_type) != keywords.end();
	};
}

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

	// expr
	ast::Expr* parsePrefixExpr(sp::Token* tok);
	ast::Expr* parseInfixExpr(ast::Expr*);
	int peekPrecedence();
	int currPrecedence();
};
