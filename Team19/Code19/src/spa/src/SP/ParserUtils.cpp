#pragma once
#include "SP/ParserUtils.h"

// function pointers for ExprParsing
namespace ParserUtils {

	const bool hasExprRank(sp::Token::TokenType tok_type) {
		return exprRanks.find(tok_type) != exprRanks.end();
	}
	const int getExprRankUnsafe(sp::Token::TokenType tok_type) {
		return exprRanks.at(tok_type);
	}

	const bool isKeyword(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return keywords.find(tok_type) != keywords.end();
	};

	const bool isInExpr(sp::Token::TokenType tok_type) {
		if (isKeyword(tok_type)) { return true; }
		if (hasExprRank(tok_type)) { return true; }
		return inExpr.find(tok_type) != inExpr.end();
	}

	void LexerStubAdapt(std::vector<sp::Token>& tok, std::vector<sp::Token*>& out) {
		for (int i = 0; i < tok.size(); ++i) {
			out.push_back(&tok[i]);
		}
	}

	void StringToTokenPtrs(std::string input, std::vector<sp::Token>& actualTokens, std::vector<sp::Token*>& out) {
		bool flag = Lexer::tokenise(input, actualTokens);
		if (!flag) {
			throw "Lexer::tokenize detected an error";
		}
		ParserUtils::LexerStubAdapt(actualTokens, out);
		if (out.size() != actualTokens.size()) {
			throw "Output Token* vector is not the same size as Lexer::tokenize vector output size";
		}
	}

	const bool isRelOps(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return rel_ops.find(tok_type) != rel_ops.end();
	};

	const bool isCondExprOps(sp::Token::TokenType tok_type) {
		// if iterator returned from .find() == .end(), that means not found in set
		return cond_expr_ops.find(tok_type) != cond_expr_ops.end();
	};
}
