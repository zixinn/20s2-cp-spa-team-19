#pragma once
#include <unordered_map>
#include <unordered_set>
#include "SP/Token.h"
#include "SP/Lexer.h"

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
}
