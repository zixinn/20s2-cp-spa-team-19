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

	// for use in ParseRelExpr
	void CheckExpr(std::vector<sp::Token*> input, std::string comp) {
		// begin ritual
        auto l = new LexerStub(input);
        auto p = Parser(l);
        // Parser now ready for use
		try {
			ast::Expr* ce = p.parseExpr(ParserUtils::ExprPrecedence::LOWEST);
		}
		catch (sp::ParserException& ex) {
			throw sp::UtilsException("" + comp + " :: " + ex.what());
		}
	}

	// expect leftmost and rightmost to be brackets
	void ParseRelExpr(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int index) {

		//std::cout << "IN: " + std::to_string(index) << std::endl;
		int left = index - 1;
		int right = index + 1;

		// go left until find (, l_lparen == 1 when we have an extra (
		int l_lparen = 0;
		while (left > 0) {
			if (input[left]->getType() == sp::Token::TokenType::RPAREN) { l_lparen--; }
			if (input[left]->getType() == sp::Token::TokenType::LPAREN) { l_lparen++; }
			if (l_lparen == 1) { break; }
			left--;
		}
		//std::cout << "LEFT: " + std::to_string(left) << std::endl;
		//std::cout << "l_lparen: " + std::to_string(l_lparen) << std::endl;

		// if not LPAREN, may have reached start of list
		if (input[left]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseRelExpr (LPAREN) expected LPAREN instead encountered: " + input[left]->getLiteral());
		}

		// go right until find ), r_lparen == -1 when we have an extra )
		int r_lparen = 0;
		while (right < input.size()) {
			if (input[right]->getType() == sp::Token::TokenType::RPAREN) { r_lparen--; }
			if (input[right]->getType() == sp::Token::TokenType::LPAREN) { r_lparen++; }
			if (r_lparen == -1) { break; }
			right++;
		}
		std::cout << "RIGHT: " + std::to_string(right) << std::endl;

		// if not RPAREN, may have reached end of list
		if (input[right]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("ParseRelExpr (RPAREN) expected RPAREN instead encountered: " + input[right]->getLiteral());
		}

		// LHS expr check, left+1 to not include (
		std::vector<sp::Token*> tmp_vec_lhs;
		VectorSlice(input, tmp_vec_lhs, left+1, index-1);
		CheckExpr(tmp_vec_lhs, "ParseRelExpr (LHS)");

		// RHS expr check, right-1 to not inclide )
		std::vector<sp::Token*> tmp_vec_rhs;
		VectorSlice(input, tmp_vec_rhs, index+1, right-1);
		CheckExpr(tmp_vec_rhs, "ParseRelExpr (RHS)");

		// copy over into return vector
		// DO NOT consume the ( and ), relExpr is valid without them, its for the others to consume
		for (int i = 0; i < input.size(); i++) {
			// this copis brackets too
			if (i <= left || right <= i) {
				output.push_back(input[i]);
			}
			if (i == index) {
				//output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "CondExprUtils.h"));
				output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "BOOL"));
			}
		}

		std::cout << "CHECKEXPR: " + std::to_string(3) << std::endl;
	}


	// outermost logic
	ast::CondExprBag ParseCondExpr(std::vector<sp::Token*>& input) {
		// find all relexpr and deal with those
		// then call ParseCondExprInner, which dispatches between NOT and &&
		throw "NOT READY";
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
		// ParseCondExprInner(input, tmp_vec);	// no relexpr anymore all done in first pass
		// if no error, then we can toss away tmp_vec
		// copy input into output vector

		// !((flag == 5 || flag > 5) && (3 > 1))
		// !((BOOL || BOOL) && (BOOL))
		// parseNot should be called last
		// put everything into a new vector and recursively call ! and && dispatch
	}

}
