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
		if (left < 0 || input.size() <= left) {
			throw sp::UtilsException("VectorSlice array out of bounds, size: " + std::to_string(input.size()) + "left: " + std::to_string(left));
		}
		if (right < 0 || input.size() <= right) {
			throw sp::UtilsException("VectorSlice array out of bounds, size: " + std::to_string(input.size()) + "right: " + std::to_string(right));
		}
		if (left > right) {
			throw sp::UtilsException("VectorSlice left greater than right, left: " + std::to_string(left) + "right: " + std::to_string(right));
		}
		for (int i = left; i <= right; ++i) {
			output.push_back(input[i]);
		}
	}

	void VectorShallowCopy(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output) {
		for (int i = 0; i < input.size(); ++i) {
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

		// if we start_lparen = 1, then we dont want the first ) to count
		//int left = start_lparen == 0 ? left_start : left_start - 1;
		int left = left_start;
		//for (int left = left_start; left >= 0; --left) {
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
		int right_mirror = right_start;

		// if start_lparen is 1, we should not let the first ( modify l_paren
		for (int right = start_lparen == 0 ? right_start : right_start + 1; right < input.size(); ++right) {
			right_mirror = right;
		//while (right < input.size()) {
			if (input[right]->getType() == sp::Token::TokenType::RPAREN) { l_paren--; }
			if (input[right]->getType() == sp::Token::TokenType::LPAREN) { l_paren++; }
			//std::cout << "right: " + to_string(right) + ",r_mirror: " + to_string(right_mirror) + ", l_paren: " + to_string(l_paren) << std::endl;
			if (l_paren == target_lparen) { break; }
		}
		//std::cout << "FINAL: r_mirror: " + to_string(right_mirror) + ", l_paren: " + to_string(l_paren) << std::endl;
		return right_mirror;
	}

	// checks if its ((((BOOL)))) or otherwise
	bool CheckSubExpr(std::vector<sp::Token*>& input) {

		if (input.size() == 1) {
			return input[0]->getType() == sp::Token::TokenType::BOOL;
		}


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
			throw sp::UtilsException("checkSubExpr (end) not at end , encountered index at: " + std::to_string(it) + ", size: " + std::to_string(input.size()) + ", vector: " + VectorToString(input));
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

	// checks if its ((((BOOL)))) or otherwise
	// but no exceptions thrown
	bool CheckSubExprNoThrow(std::vector<sp::Token*>& input) {
		bool out = false;
		try {
			out = CheckSubExpr(input);
		}
		catch (sp::UtilsException& ex) {
			// out = false;
		}
		return out;
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

		// ensure no array out of bounds exception
		if (right_end >= input.size()) {
			throw sp::UtilsException("SeekParenRightCheck (Seek right_end) out of bounds, right_end: " + std::to_string(right_end) + ", size: " + std::to_string(input.size()));
		}
		if (input[right_end]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("SeekParenRightCheck (right_end) expected RPAREN instead encountered: " + input[right_end]->getLiteral());
		}

		// get a copy of the       && ( ... ), the  ( ... ) inclusive
		std::vector<sp::Token*> tmp_right;
		VectorSlice(input, tmp_right, right_start, right_end);
		//VectorSlice(input, tmp_right, right_start+1, right_end-1);
		//std::cout << "right_start: " + std::to_string(right_start) + ", right_end: " + std::to_string(right_end) << std::endl;
		//std::cout << "size of tmp_right: " + std::to_string(tmp_right.size()) + ", vector: " + VectorToString(tmp_right) << std::endl;

		// recursively deal with what is in thr RHS && ( ... )
		// CondExprDispatch must return the outermost ( ... )
		std::vector<sp::Token*> new_right;
		CondExprDispatch(tmp_right, new_right);
		std::cout << "SeekParenRight:: tmp_right: " + VectorToString(tmp_right) + ", new_right: " + VectorToString(new_right) << std::endl;

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
		std::cout << "CAR BEFORE:: Input: " + VectorToString(input) + ", Output: " + VectorToString(output) + ", left: " + to_string(left) + ", index: " + to_string(index) + ", right: " + to_string(right) << endl;
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
		std::cout << "CAR AFTER:: Input: " + VectorToString(input) + ", Output: " + VectorToString(output) + ", left: " + to_string(left) + ", index: " + to_string(index) + ", right: " + to_string(right) << endl;
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
		std::vector<sp::Token*> tmp_in;
		std::vector<sp::Token*> tmp_out;
		VectorShallowCopy(input, tmp_in);
		int ii = 0;

		while (tmp_in.size() != tmp_out.size()) {
			bool isClean = true;
			tmp_out.clear();
			cout << "DISPATCH:: tmp_in: " + VectorToString(tmp_in) + ", ii: " + to_string(ii) << endl;
			for (int i = 0; i < tmp_in.size(); i++) {
				auto tok = tmp_in[i];
				if (ParserUtils::isCondExprOps(tok->getType())) {
					ParseAndOr(tmp_in, tmp_out, i);
					isClean = false;
					break;	// out of for, still in while
				} else if (tok->getType() == sp::Token::TokenType::NOT) {
					ParseNot(tmp_in, tmp_out, i);
					isClean = false;
					break;	// out of for, still in while
				}
			}
			cout << "DISPATCH Exit FOR" << endl;
			if (isClean) { 
				// untouched, so tmp_out would be empty and we should copy it over
				VectorShallowCopy(tmp_in, tmp_out);
				break; 
			}
			if (CheckSubExprNoThrow(tmp_out)) { break; }
			cout << "DISPATCH RESET BEGIN" << endl;
			// reset
			tmp_in.clear();
			VectorShallowCopy(tmp_out, tmp_in);
			tmp_out.clear();
			ii++;
		}
		VectorShallowCopy(tmp_out, output);
	}

	//void CondExprDispatch(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output) {
	//	int ceIndex = -1;
	//	bool isDirty = false;
	//	for (int i = 0; i < input.size(); i++) {
	//		auto tok = input[i];
	//		if (ParserUtils::isCondExprOps(tok->getType())) {
	//			ceIndex = i;
	//			std::cout << "DISPATCH ANDOR (before):: ceIndex: " + std::to_string(ceIndex) + ", vector: " + VectorToString(input) + ", output: " + VectorToString(output) << std::endl;
	//			ParseAndOr(input, output, ceIndex);
	//			std::cout << "DISPATCH ANDOR (after):: ceIndex: " + std::to_string(ceIndex) + ", vector: " + VectorToString(input) + ", output: " + VectorToString(output) << std::endl;
	//			return;
	//			isDirty = true;
	//		} else if (tok->getType() == sp::Token::TokenType::NOT) {
	//			ceIndex = i;
	//			std::cout << "DISPATCH NOT (before) :: ceIndex: " + std::to_string(ceIndex) + ", vector: " + VectorToString(input) + ", output: " + VectorToString(output) << std::endl;
	//			ParseNot(input, output, ceIndex);
	//			std::cout << "DISPATCH NOT (after) :: ceIndex: " + std::to_string(ceIndex) + ", vector: " + VectorToString(input) + ", output: " + VectorToString(output) << std::endl;
	//			return;
	//			isDirty = true;
	//		}
	//	}
	//	//std::cout << "HERE: " << std::endl;
	//	// cannot find any arguments, expect (BOOL), so just copy it out, let caller check
	//	if (!isDirty) {
	//		VectorShallowCopy(input, output);
	//	}
	//}

	// outermost logic
	ast::CondExprBag ParseCondExpr(std::vector<sp::Token*>& input) {

		// assumes extreme L and extreme R must be same enclosign ( ... ) level
		// deals with everything inside those brackets
		throw "NOT READY";
	}

	void ParseCondExprInner(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output) {
		//throw "NOT READY";
		// find all relexpr and deal with those
		// then call ParseCondExprInner, which dispatches between NOT and &&
		std::vector<sp::Token*> tmp_out;
		RelExprDispatch(input, tmp_out);
		CondExprDispatch(tmp_out, output);
	}

	void RelExprDispatch(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output) {
		std::vector<sp::Token*> tmp_in;
		std::vector<sp::Token*> tmp_out;
		VectorShallowCopy(input, tmp_in);
		int ii = 0;

		while (tmp_in.size() != tmp_out.size()) {
			bool isClean = true;
			tmp_out.clear();
			cout << "RELEXPR DISPATCH:: tmp_in: " + VectorToString(tmp_in) + ", ii: " + to_string(ii) << endl;
			for (int i = 0; i < tmp_in.size(); i++) {
				auto tok = tmp_in[i];
				if (ParserUtils::isRelOps(tok->getType())) {
					ParseRelExpr(tmp_in, tmp_out, i);
					isClean = false;
					break;	// out of for, still in while
				}
			}
			cout << "RELEXPR DISPATCH Exit FOR" << endl;
			if (isClean) { 
				// untouched, so tmp_out would be empty and we should copy it over
				VectorShallowCopy(tmp_in, tmp_out);
				break; 
			}
			// not needed since may have && or || or !
			//if (CheckSubExprNoThrow(tmp_out)) { break; }
			cout << "RELEXPR DISPATCH RESET BEGIN" << endl;
			// reset
			tmp_in.clear();
			VectorShallowCopy(tmp_out, tmp_in);
			tmp_out.clear();
			ii++;
		}
		VectorShallowCopy(tmp_out, output);
	}
		

	// index is the index of the && operator
	void ParseAndOr(std::vector<sp::Token*>& input,std::vector<sp::Token*>& output, int index) {
		int left_rparen = index - 1;
		int right_lparen = index + 1;
		std::cout << "ParseAndOr: " + std::to_string(right_lparen) + ", Vector: " + VectorToString(input) + ", Output: " + VectorToString(output) << std::endl;

		// Index has to be && of ||
		if (!ParserUtils::isCondExprOps(input[index]->getType())) {
			throw sp::UtilsException("ParseAndOr (Index) expected && or || instead encountered: " + input[index]->getLiteral());
		}

		// immediate Left of && has to be first RParen
		// ... ) &&
		if (input[left_rparen]->getType() != sp::Token::TokenType::RPAREN) {
			throw sp::UtilsException("ParseAndOr (LHS) expected RPAREN instead encountered: " + input[left_rparen]->getLiteral());
		}
		int left_lparen = SeekParenLeftCheck(input, left_rparen);
		std::cout << "ParseAndOr :: left_lparen: " + std::to_string(left_lparen) + ", vector: " + VectorToString(input) << std::endl;

		// immediate Right of && has to be first LParen
		//       && ( ...
		if (input[right_lparen]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseAndOr (RHS) expected LPAREN instead encountered: " + input[right_lparen]->getLiteral());
		}

		std::vector<sp::Token*> tmp_right;
		int right_rparen = SeekParenRightCheck(input, tmp_right, right_lparen);

		// ( ... ) && ( ... ) replace with
		// BOOL
		CopyAndReplace(tmp_right, output, left_lparen, index, right_rparen);

	}

	void ParseNot(std::vector<sp::Token*>& input,std::vector<sp::Token*>& output, int index) {
		int right_lparen = index + 1;

		// Index has to be !
		if (input[index]->getType() != sp::Token::TokenType::NOT) {
			throw sp::UtilsException("ParseNot (Index) expected ! instead encountered: " + input[index]->getLiteral());
		}

		// immediate right of ! has to be first LParen
		// ! ( ...
		if (input[right_lparen]->getType() != sp::Token::TokenType::LPAREN) {
			throw sp::UtilsException("ParseNot (RHS) expected LPAREN instead encountered: " + input[right_lparen]->getLiteral());
		}
		std::cout << "ParseNot: " + std::to_string(right_lparen) + ", Vector: " + VectorToString(input) + ", Output: " + VectorToString(output) << std::endl;

		std::vector<sp::Token*> tmp_right;
		int right_rparen = SeekParenRightCheck(input, tmp_right, right_lparen);
		std::cout << "ParseNot: tmp_right :: " + VectorToString(tmp_right) << std::endl;

		// !( ... ) replace with
		// BOOL
		CopyAndReplace(tmp_right, output, index, index, right_rparen);
	}

}
