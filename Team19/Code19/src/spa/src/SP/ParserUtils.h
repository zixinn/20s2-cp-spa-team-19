#pragma once
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "SP/Token.h"
#include "SP/Lexer.h"
#include "SP/ParserException.h"
#include "AST/Program.h"
#include "AST/Proc.h"
#include "AST/ProcName.h"
#include "AST/Stmt.h"
#include "AST/StmtLst.h"
#include "AST/IfStmt.h"
#include "AST/WhileStmt.h"
#include "AST/CallStmt.h"

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

	// CondExpr precedence, higher the more impt
	namespace CondExprPrecedence {		// the order matters
		const int BLANK = 0;
		const int LOWEST = 1;
		const int ANDOR = 2;		// operators: && ||
		const int PREFIX = 3;		// operators: !
		const int GLTEQN = 4;		// > >= < <= == !=
	};

	// map tokens to correct BODMAS precedence ranking
	const unordered_map<sp::Token::TokenType, int, sp::tokentype_hash> exprRanks{
		{sp::Token::TokenType::PLUS, ExprPrecedence::ADDMINUS },
		{sp::Token::TokenType::MINUS, ExprPrecedence::ADDMINUS },
		{sp::Token::TokenType::DIV, ExprPrecedence::DIVMULT },
		{sp::Token::TokenType::TIMES, ExprPrecedence::DIVMULT },
		{sp::Token::TokenType::MOD, ExprPrecedence::DIVMULT },
	};

	const bool hasExprRank(sp::Token::TokenType tok_type);
	const int getExprRankUnsafe(sp::Token::TokenType tok_type);

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

	const bool isKeyword(sp::Token::TokenType tok_type);

	// to determine what can pass as an Expr, Keyword or NAME or CONST or Operators or semicolon
	// for CondExprUtils.h
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> inExpr {
		// keywords
		sp::Token::TokenType::PROC,
		sp::Token::TokenType::READ,
		sp::Token::TokenType::PRINT,
		sp::Token::TokenType::CALL,
		sp::Token::TokenType::WHILE,
		sp::Token::TokenType::IF,
		sp::Token::TokenType::THEN,
		sp::Token::TokenType::ELSE,

		// names and const
		sp::Token::TokenType::NAME,
		sp::Token::TokenType::CONST,

		// Operators
		sp::Token::TokenType::PLUS,
		sp::Token::TokenType::MINUS,
		sp::Token::TokenType::DIV,
		sp::Token::TokenType::TIMES,
		sp::Token::TokenType::MOD,

		// paren
		sp::Token::TokenType::LPAREN,
		sp::Token::TokenType::RPAREN,

	};

	const bool isInExpr(sp::Token::TokenType tok_type);

	void LexerStubAdapt(std::vector<sp::Token>& tok, std::vector<sp::Token*>& out);

	void StringToTokenPtrs(std::string input, std::vector<sp::Token>& actualTokens, std::vector<sp::Token*>& out);

	// EQ, NEQ, GT, GTE, LT, LTE,  //rel expr
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> rel_ops {
		sp::Token::TokenType::EQ,
		sp::Token::TokenType::NEQ,
		sp::Token::TokenType::GT,
		sp::Token::TokenType::GTE,
		sp::Token::TokenType::LT,
		sp::Token::TokenType::LTE,
	};

	const bool isRelOps(sp::Token::TokenType tok_type);

	// AND, OR,  cond_expr ops
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> cond_expr_ops {
		sp::Token::TokenType::AND,
		sp::Token::TokenType::OR,
	};

	const bool isCondExprOps(sp::Token::TokenType tok_type);

	// does not check for call to procedures that do not exist, for that use hasUndefCall
	const bool isASTCyclic(ast::Program* prog, std::unordered_set<std::string>& path);
	const void throwIfCyclic(ast::Program* prog);
	const bool cyclicDfs(std::string proc, std::unordered_set<std::string>& visited, std::unordered_map<std::string, std::vector<string>>& adjL, std::unordered_set<std::string>& rec_stack);
	const void getCallsInProc(std::vector<ast::Stmt*>, std::vector<std::string>&);

	const void throwIfUndefCall(ast::Program* prog);
	const bool hasUndefCall(ast::Program* prog, std::unordered_set<std::string>& undef_calls);
	const void hasUndefCall_helper(std::vector<ast::Stmt*> stmts, std::unordered_set<std::string>& def_proc, std::unordered_set<std::string>& undef_call);

	const std::string setToString(std::unordered_set<std::string>& path);
}
