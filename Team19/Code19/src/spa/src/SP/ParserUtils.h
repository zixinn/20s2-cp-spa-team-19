#pragma once
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

	inline void LexerStubAdapt(std::vector<sp::Token>& tok, std::vector<sp::Token*>& out) {
		for (int i = 0; i < tok.size(); ++i) {
			out.push_back(&tok[i]);
		}
	}

	inline void StringToTokenPtrs(std::string input, std::vector<sp::Token>& actualTokens, std::vector<sp::Token*>& out) {
		bool flag = Lexer::tokenise(input, actualTokens);
		if (!flag) {
			throw "Lexer::tokenize detected an error";
		}
		ParserUtils::LexerStubAdapt(actualTokens, out);
		if (out.size() != actualTokens.size()) {
			throw "Output Token* vector is not the same size as Lexer::tokenize vector output size";
		}
	}

	// EQ, NEQ, GT, GTE, LT, LTE,  //rel expr
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> rel_ops {
		sp::Token::TokenType::EQ,
		sp::Token::TokenType::NEQ,
		sp::Token::TokenType::GT,
		sp::Token::TokenType::GTE,
		sp::Token::TokenType::LT,
		sp::Token::TokenType::LTE,
	};

	inline const bool isRelOps(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return rel_ops.find(tok_type) != rel_ops.end();
	};

	// AND, OR,  cond_expr ops
	const std::unordered_set<sp::Token::TokenType, sp::tokentype_hash> cond_expr_ops {
		sp::Token::TokenType::AND,
		sp::Token::TokenType::OR,
	};

	inline const bool isCondExprOps(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return cond_expr_ops.find(tok_type) != cond_expr_ops.end();
	};
}
