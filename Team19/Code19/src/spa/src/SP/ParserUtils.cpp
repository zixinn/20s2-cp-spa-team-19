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

	const bool isASTCyclic(ast::Program* prog, std::unordered_set<std::string>& path) {

		// build Adjacency List of Procedures to Who they call
		std::unordered_map<std::string, std::vector<std::string>> adjL{};
		vector<ast::Proc*> procs = prog->getProcedures();
		for (int i = 0; i < procs.size(); i++) {
			ast::Proc* procedure = procs[i];
			vector<ast::Stmt*> stmts = procedure->getStmtLst()->getStatements();
			std::vector<std::string> neighbors;
			getCallsInProc(stmts, neighbors);
			adjL.insert({ procedure->getName()->getVal(), neighbors });
		}

		// begin dfs for every procedure
		std::unordered_set<std::string> visited;
		for (int i = 0; i < procs.size(); i++) {
			ast::Proc* procedure = procs[i];
			std::unordered_set<std::string> rec_stack;
			std::string root_proc = procedure->getName()->getVal();
			//std::cout << "Count: " + std::to_string(i) + ", Root: " + root_proc << std::endl;
			bool is_cycle = cyclicDfs(root_proc, visited, adjL, rec_stack);
			if (is_cycle) { 
				// WARNING: this is not a java reference change, this overloaded operator does shallow copy
				path = rec_stack;	
				return true; 
			}
		}
		return false;
	}

	const bool cyclicDfs(std::string proc, std::unordered_set<std::string>& visited, std::unordered_map<std::string, std::vector<string>>& adjL, std::unordered_set<std::string>& rec_stack) {
        bool is_visited = visited.find(proc) != visited.end();
		//if (is_visited) { return true;  }
		if (is_visited) { return false;  }
		if (!is_visited) { visited.insert(proc); }

		// path taken
		rec_stack.insert(proc);
		//std::cout << "RecStack: " + setToString(rec_stack) << std::endl;

		// if proc did not call any other procs, then it is not cyclic
        bool has_neighbors = adjL.find(proc) != adjL.end();
		if (!has_neighbors) { return false; }

		auto neighbors = adjL.find(proc)->second;
		for (int i = 0; i < neighbors.size(); ++i) {
			auto neighbor = neighbors[i];

			// visiting a neighbor which has is part of the recursive stack (making up this path) 
			// makes this a cycle
			bool neighbor_visited = visited.find(neighbor) != visited.end();
			bool neighbor_in_rec_stack = rec_stack.find(neighbor) != rec_stack.end();
			if (neighbor_visited && neighbor_in_rec_stack) { return true;  }

			// calling a proc which is in a cycle, makes this proc also part of a cycle
			if (cyclicDfs(neighbor, visited, adjL, rec_stack)) {
				return true;
			}
		}

		// pop proc from the stack as we are done exploring all paths that goes through it
		rec_stack.erase(proc);
		return false;
	}

	const void getCallsInProc(std::vector<ast::Stmt*> stmts, std::vector<std::string>& out) {
		for (int i = 0; i < stmts.size(); i++) {
			ast::Stmt* stmt = stmts[i];
			
			sp::Token::TokenType type = stmt->getToken()->getType();
			if (type == sp::Token::TokenType::CALL) {
				//ast::AssignStmt* ass = (ast::AssignStmt*)stmt;
				ast::CallStmt* cs = (ast::CallStmt*)stmt;
				out.push_back(cs->getName()->getVal());
			} else if (type == sp::Token::TokenType::WHILE) {
				ast::WhileStmt* whi = (ast::WhileStmt*)stmt;
				//ast::CondExprBag* ce = (ast::CondExprBag*) whi->getCondExpr();
				vector<ast::Stmt*> whileStmts = whi->getStmtLst()->getStatements();
				getCallsInProc(whileStmts, out);
				//addStmtLstToDE(whileStmts);
			} else if (type == sp::Token::TokenType::IF) {

				ast::IfStmt* iff = (ast::IfStmt*)stmt;
				//ast::CondExprBag* ce = (ast::CondExprBag*)iff->getCondExpr();
				vector<ast::Stmt*> ifCon = iff->getConsequence()->getStatements();
				getCallsInProc(ifCon, out);
				//addStmtLstToDE(ifCon);

				vector<ast::Stmt*> ifAlt = iff->getAlternative()->getStatements();
				getCallsInProc(ifAlt, out);
				//addStmtLstToDE(ifAlt);
			} else {
				continue;
			}
		}
	}

	// breaks early, if one procedure contains undefined calls, will not check other procedures
	const bool hasUndefCall(ast::Program* prog, std::unordered_set<std::string>& undef_calls) {
		// build set of Procedures that were defined in the program
		std::unordered_set<std::string> def_procs{};
		vector<ast::Proc*> procs = prog->getProcedures();
		for (int i = 0; i < procs.size(); i++) {
			ast::Proc* procedure = procs[i];
			std::string proc_name = procedure->getName()->getVal();
			def_procs.insert(proc_name);
		}

		// get set of calls which are not available
		for (int i = 0; i < procs.size(); i++) {
			ast::Proc* procedure = procs[i];
			vector<ast::Stmt*> stmts = procedure->getStmtLst()->getStatements();
			hasUndefCall_helper(stmts, def_procs, undef_calls);
			if (undef_calls.size() > 0) { return true; }
		}
		return false;
	}

	// DFS for calls
	const void hasUndefCall_helper(std::vector<ast::Stmt*> stmts, std::unordered_set<std::string>& def_proc, std::unordered_set<std::string>& undef_calls) {
		for (int i = 0; i < stmts.size(); i++) {
			ast::Stmt* stmt = stmts[i];
			
			sp::Token::TokenType type = stmt->getToken()->getType();
			if (type == sp::Token::TokenType::CALL) {
				ast::CallStmt* cs = (ast::CallStmt*)stmt;
				std::string proc = cs->getName()->getVal();
				bool is_defined = def_proc.find(proc) != def_proc.end();

				// calling a procedure that has not been defined
				if (!is_defined) { undef_calls.insert(proc); }
			} else if (type == sp::Token::TokenType::WHILE) {
				ast::WhileStmt* whi = (ast::WhileStmt*)stmt;
				vector<ast::Stmt*> whileStmts = whi->getStmtLst()->getStatements();
				hasUndefCall_helper(whileStmts, def_proc, undef_calls);
			} else if (type == sp::Token::TokenType::IF) {
				ast::IfStmt* iff = (ast::IfStmt*)stmt;
				vector<ast::Stmt*> ifCon = iff->getConsequence()->getStatements();
				hasUndefCall_helper(ifCon, def_proc, undef_calls);

				vector<ast::Stmt*> ifAlt = iff->getAlternative()->getStatements();
				hasUndefCall_helper(ifAlt, def_proc, undef_calls);
			} else {
				continue;
			}
		}
	}

	const std::string setToString(std::unordered_set<std::string>& path) {
		std::string out = "";
		bool isFirst = true;
		for (auto proc : path) {
			if (isFirst) {
				out += proc;
				isFirst = false;
			}
			else {
				out = out + ", " + proc;
			}
		}
		return out;
	}

	const void throwIfCyclic(ast::Program* prog) {
        std::unordered_set<std::string> out;
        bool result = isASTCyclic(prog, out);
		if (result) {
			throw sp::UtilsException("Program has cyclical call: " + setToString(out) + "\n\n");
		}
	}

	const void throwIfUndefCall(ast::Program* prog) {
        std::unordered_set<std::string> out;
        bool result = hasUndefCall(prog, out);
		if (result) {
			throw sp::UtilsException("Program called an undefined procedure: " + *begin(out) + "\n\n");
		}
	}
}
