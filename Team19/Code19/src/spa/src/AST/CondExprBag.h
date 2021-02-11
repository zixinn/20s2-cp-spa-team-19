#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/VarName.h"
#include "AST/ConstVal.h"

// Interface CondExpr for booleans
namespace ast {
	class CondExprBag : public CondExpr
	{
		std::unordered_set<std::string> var_names;
		std::unordered_set<std::string> const_vals;
		//std::vector<std::string> vns;
		//std::vector<std::string> cvs;
		std::vector<sp::Token*> tokens;
	public:
		CondExprBag(sp::Token* token) :CondExpr(token) {};
		inline std::string toString() {
			std::string out = "";
			//std::string out = "BAG CONTENTS:\n";
			for (int i = 0; i < tokens.size(); ++i) {
				out += tokens[i]->getLiteral();
				if (i < tokens.size() - 1) {
					out += " ";
				}
			}
			return out;
		};
		inline std::vector<std::string> getVarNames() { 
			std::vector<std::string> vns;
			for (std::string vn : this->var_names) {
				vns.push_back(vn);
			}
			return vns;
		}
		inline std::vector<std::string> getConstVal() { 
			std::vector<std::string> cvs;
			for (std::string cv : this->const_vals) {
				cvs.push_back(cv);
			}
			return cvs;
		}
		inline std::vector<sp::Token*> getTokens() { return this->tokens; }
		inline void pushVarName(std::string vn) { this->var_names.insert(vn);  }
		inline void pushConstVal(std::string cv) { this->const_vals.insert(cv);  }
		inline void pushToken(sp::Token* tok) { 
			if (tok->getType() == sp::Token::TokenType::NAME) {
				auto vn = tok->getLiteral();
				pushVarName(vn);
			}
			else if (tok->getType() == sp::Token::TokenType::CONST) {
				auto cv = tok->getLiteral();
				pushConstVal(cv);
			}
			this->tokens.push_back(tok);
		}
		~CondExprBag() {
			var_names.clear();
			const_vals.clear();
			for (int i = 0; i < tokens.size(); ++i) {
				delete tokens[i];
			}
			const_vals.clear();
		}
	};
}
