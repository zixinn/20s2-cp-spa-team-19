#pragma once
#include <vector>
#include "SP/Token.h"
#include "SP/Lexer.h"
#include "SP/Parser.h"
#include "SP/ParserException.h"

// function pointers for ExprParsing
namespace CondExprUtils {


	std::string VectorToString(std::vector<sp::Token*>& input) {
		std::string out = "";
		for (int i = 0; i < input.size(); ++i) {
			out += input[i]->getLiteral();
			if (i >= input.size() - 1) { continue; }
			out += " ";
		}
		return out;
	}

	void VectorSlice(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int right) {
		if (input.size() <= left) {
			throw sp::UtilsException("VectorSlice array out of bounds, size: " + std::to_string(input.size()) + "left: " + std::to_string(left));
		}
		if (input.size() <= right) {
			throw sp::UtilsException("VectorSlice array out of bounds, size: " + std::to_string(input.size()) + "right: " + std::to_string(right));
		}
		for (int i = left; i <= right; ++i) {
			output.push_back(input[i]);
		}
	}

	void ParseNot(std::vector<sp::Token*>& input,std::vector<sp::Token*>& output, int index) {
		int left = index + 1;
		int right = left + 1;

		// immediate right of ! has to be first LParen
		if (input[left]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseNot expected LPAREN instead encountered: " + input[left]->getLiteral());
		}

		int l_paren = 1;
		for (right = left + 1; right < input.size(); right++) {
			if (input[right]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
			if (input[right]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
			if (l_paren <= 0) { break; }
		}

		// if not RPAREN, may have reached end of list
		if (input[right]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("ParseNot expected RPAREN instead encountered: " + input[right]->getLiteral());
		}

		std::vector<sp::Token*> tmp_vec;
		// ParseRelExprAlter(input, tmp_vec);
		// if no error, then we can toss away tmp_vec
		// copy input into output vector

		// !((flag == 5 || flag > 5) && (3 > 1))
		// !((BOOL || BOOL) && (BOOL))
		// parseNot should be called last
		// put everything into a new vector and recursively call ! and && dispatch
	}

}
