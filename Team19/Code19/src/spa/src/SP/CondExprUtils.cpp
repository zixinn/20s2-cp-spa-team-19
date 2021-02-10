#pragma once
#include "SP/CondExprUtils.h"

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

	// l_paren++ for LParen, l_paren-- for RParen
	// left_start: where to start moving left from inclusive
	// example lparen: start:  0, target:  1 := `( ...   left_start` for <
	// example lparen: start: -1, target:  0 := `( ... ) left_start` for &&
	// returns: index of found or start of list, do your own check
	int SeekParenLeft(std::vector<sp::Token*>& input, int left_start, int start_lparen, int target_lparen) {
		// go left until find (, l_lparen == 1 when we have an extra (
		//int l_lparen = 0;
		int l_lparen = start_lparen;
		int left = left_start;
		while (left > 0) {
			if (input[left]->getType() == sp::Token::TokenType::RPAREN) { l_lparen--; }
			if (input[left]->getType() == sp::Token::TokenType::LPAREN) { l_lparen++; }
			if (l_lparen == target_lparen) { break; }
			//if (l_lparen == 1) { break; }
			left--;
		}
		return left;
	}

	// example lparen: start:  0, target: -1 := `right   ... )` for <
	// example lparen: start:  1, target:  0 := `right ( ... )` for &&
	int SeekParenRight(std::vector<sp::Token*>& input, int right_start, int start_lparen, int target_lparen) {
		int l_paren = start_lparen;
		int right = right_start;
		while (right < input.size()) {
			if (input[right]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
			if (input[right]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
			if (l_paren == target_lparen) { break; }
			right++;
		}
		return right;
	}

	// checks if its ((((BOOL)))) or otherwise
	bool CheckSubExpr(std::vector<sp::Token*>& input) {
		//int it = input.size() - 1;
		int it = 0;
		// iterator start pos must be )
		if (input[it]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("checkSubExpr(LPAREN) expected LPAREN instead encountered: " + input[it]->getLiteral());
		}

		int l_paren = 0;
		int target_lparen = 0;
		while (it < input.size() && input[it]->getType() == sp::Token::TokenType::LPAREN) {
			if (input[it]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
			if (input[it]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
			if (l_paren == target_lparen) { break; }
			it++;
		}

		// expect BOOL
		if (input[it]->getType() != sp::Token::TokenType::BOOL) {
			throw sp::UtilsException("checkSubExpr (BOOL) expected BOOL instead encountered: " + input[it]->getLiteral());
		}
		// shift away from BOOL after being satisfied
		it++;

		while (it < input.size() && input[it]->getType() == sp::Token::TokenType::RPAREN) {
			if (input[it]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
			if (input[it]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
			if (l_paren == target_lparen) { break; }
			it++;
		}

		// could have terminated earlier (BOOL)) or reached the end of array but no l_paren ((BOOL)
		if (it != input.size() - 1) {
			throw sp::UtilsException("checkSubExpr (end) not at end , encountered index at: " + std::to_string(it));
		}

		// expect l_paren to be target_lparen
		if (input[input.size()-1]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("checkSubExpr (RPAREN) expected RPAREN instead encountered: " + input[input.size()-1]->getLiteral());
		}

		if (l_paren != target_lparen) {
			throw sp::UtilsException("checkSubExpr (target) not at target, encountered: " + std::to_string(l_paren));
		}
		//std::cout << "index at: " + std::to_string(it) + "l_paren: " + std::to_string(l_paren) << std::endl;

		// pass all checks
		return true;
	}


	// example lparen: start: -1, target:  0 := `( ... ) left_start` for &&
	// returns: index of found or start of list (, do your own check
	int SeekParenLeftCheck(std::vector<sp::Token*>& input, int left_start) {
		int left = left_start; // the rightmost )

		// if left 
		if (left < 0 || input.size() <= left) {
			throw sp::UtilsException("SeekParenLCheck left out of bounds: " + std::to_string(left));
		}

		// left_start must be )
		if (input[left]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("SeekParenLCheck (RPAREN) expected RPAREN instead encountered: " + input[left]->getLiteral());
		}

		// seek ( ... ) &&
		int left_end = SeekParenLeft(input, left_start-1,-1,0);
		std::cout << "KEFTL :: " + std::to_string(left_end) << std::endl;
		std::vector<sp::Token*> tmp;
		VectorSlice(input, tmp, left_end, left_start);

		// check if legal
		CheckSubExpr(tmp);
		return left_end;
	}

	// example lparen: start: -1, target:  0 := `( ... ) left_start` for &&
	// returns: index of found or start of list (, do your own check
	// right_start is the ( paren position
	int SeekParenRightCheck(std::vector<sp::Token*>& input, std::vector<sp::Token*>&output, int right_start) {

		// right_end is the index of corresponding ) for right_start's (
		int right_end = SeekParenRight(input, right_start, 1, 0);

		// get a copy of the       && ( ... ), the  ( ... ) inclusive
		std::vector<sp::Token*> tmp_right;
		VectorSlice(input, tmp_right, right_start, right_end);

		// recursively deal with what is in thr RHS && ( ... )
		// CondExprDispatch must return the outermost ( ... )
		std::vector<sp::Token*> new_right;
		CondExprDispatch(tmp_right, new_right);

		// now new_right has been fully processed, should not have any ! or &&
		CheckSubExpr(new_right);

		// copy everything on old LHS up till && incl, ( HERE ) &&
		for (int i = 0; i < right_start; i++) {
			output.push_back(input[i]);
		}

		// copy over new_right RHS, basically       && ( HERE ), incl Paren
		for (int i = 0; i < new_right.size(); i++) {
			output.push_back(new_right[i]);
		}

		// copy everything on the right of the RHS, ( ... ) && ( ... ) HERE
		for (int i = right_end + 1; i < input.size(); i++) {
			output.push_back(input[i]);
		}

		// find the new index of the outermost matching )
		// since list modified by CheckExprDispatch
		int right = SeekParenRight(output, right_start, 1, 0);

		return right;
	}

	// consumes L and R inclusive a,b,c,d,(,e,f,g,h,),i,j,k,l -> a,b,c,d,BOOL,i,j,k,l
	int CopyAndReplace(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int index, int right) {
		// copy over into return vector
		for (int i = 0; i < input.size(); i++) {
			// this copis brackets too
			if (i < left || right < i) {
				output.push_back(input[i]);
			}
			if (i == index) {
				//output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "CondExprUtils.h"));
				output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "BOOL"));
			}
		}
		return -1; // idk
	}

	int CopyAndReplaceExcl(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int index, int right) {
		// DO NOT consume the ( and ), relExpr is valid without them, its for the others to consume
		return CopyAndReplace(input, output, left+1, index, right-1);
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

		// example lparen: start:  0, target:  1 := `( ...   left_start` for <
		// go left until find (, l_lparen == 1 when we have an extra (
		left = SeekParenLeft(input, left, 0, 1);
		//int l_lparen = 0;
		//while (left > 0) {
		//	if (input[left]->getType() == sp::Token::TokenType::RPAREN) { l_lparen--; }
		//	if (input[left]->getType() == sp::Token::TokenType::LPAREN) { l_lparen++; }
		//	if (l_lparen == 1) { break; }
		//	left--;
		//}
		//std::cout << "LEFT: " + std::to_string(left) << std::endl;
		//std::cout << "l_lparen: " + std::to_string(l_lparen) << std::endl;

		// if not LPAREN, may have reached start of list
		if (input[left]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseRelExpr (LPAREN) expected LPAREN instead encountered: " + input[left]->getLiteral());
		}

		// example lparen: start:  0, target: -1 := `right   ... )` for <
		right = SeekParenRight(input, right, 0, -1);
		//int r_lparen = 0;
		//while (right < input.size()) {
		//	if (input[right]->getType() == sp::Token::TokenType::RPAREN) { r_lparen--; }
		//	if (input[right]->getType() == sp::Token::TokenType::LPAREN) { r_lparen++; }
		//	if (r_lparen == -1) { break; }
		//	right++;
		//}
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
		CopyAndReplaceExcl(input, output, left, index, right);
		//for (int i = 0; i < input.size(); i++) {
		//	// this copis brackets too
		//	if (i <= left || right <= i) {
		//		output.push_back(input[i]);
		//	}
		//	if (i == index) {
		//		//output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "CondExprUtils.h"));
		//		output.push_back(new sp::Token(sp::Token::TokenType::BOOL, "BOOL"));
		//	}
		//}

		std::cout << "CHECKEXPR: " + std::to_string(3) << std::endl;
	}

	void CondExprDispatch(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output) {
		int ceIndex = -1;
		for (int i = 0; i < input.size(); i++) {
			auto tok = input[i];
			if (ParserUtils::isCondExprOps(tok->getType())) {
				ceIndex = i;
				ParseAndOr(input, output, ceIndex);
				return;
			} else if (tok->getType() == sp::Token::TokenType::NOT) {
				ceIndex = i;
				ParseNot(input, output, ceIndex);
				return;
			}
		}
	}

	// outermost logic
	ast::CondExprBag ParseCondExpr(std::vector<sp::Token*>& input) {
		// find all relexpr and deal with those
		// then call ParseCondExprInner, which dispatches between NOT and &&

		// assumes extreme L and extreme R must be same enclosign ( ... ) level
		// deals with everything inside those brackets
		throw "NOT READY";
	}

	void ParseAndOr(std::vector<sp::Token*>& input,std::vector<sp::Token*>& output, int index) {
		int left_rparen = index - 1;
		int right_lparen = index + 1;

		// immediate Left of && has to be first RParen
		// ... ) &&
		if (input[left_rparen]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("ParseAndOr (LHS) expected RPAREN instead encountered: " + input[left_rparen]->getLiteral());
		}
		int left_lparen = SeekParenLeftCheck(input, left_rparen);

		// immediate Right of && has to be first LParen
		//       && ( ...
		if (input[right_lparen]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseAndOr (RHS) expected LPAREN instead encountered: " + input[right_lparen]->getLiteral());
		}

		std::vector<sp::Token*> tmp_right;
		int right_rparen = SeekParenRightCheck(input, tmp_right, right_lparen);

		// ( ... ) && ( ... ) replace with
		// (      BOOL      )
		CopyAndReplace(tmp_right, output, left_lparen, index, right_rparen);

	}

	void ParseNot(std::vector<sp::Token*>& input,std::vector<sp::Token*>& output, int index) {
		int left = index + 1;
		int right = left + 1;

		// immediate right of ! has to be first LParen
		if (input[left]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseNot expected LPAREN instead encountered: " + input[left]->getLiteral());
		}

		// goal: !( ... ), we want the ), so only need to seekRight
		// no we cant do this, we need check again for BOOL or sth
		right = SeekParenRight(input, right, 1, 0);
		//int l_paren = 1;
		//for (right = left + 1; right < input.size(); right++) {
		//	if (input[right]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
		//	if (input[right]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
		//	if (l_paren <= 0) { break; }
		//}

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
