#pragma once
#include <string>
#include <vector>
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/VarName.h"
#include "AST/ConstVal.h"

// Interface CondExpr for booleans
namespace ast {
	class CondExprBag : public CondExpr
	{
		std::vector<VarName*> var_names;
		std::vector<ConstVal*> const_vals;
		std::vector<sp::Token*> tokens;
	public:
		CondExprBag(sp::Token* token) :CondExpr(token) {};
		inline std::string toString() {
			std::string out = "BAG CONTENTS:\n";
			for (int i = 0; i < tokens.size(); ++i) {
				out += tokens[i]->getLiteral() + ", ";
			}
			return out;
		};
		inline std::vector<VarName*> getVarNames() { return this->var_names;  }
		inline std::vector<ConstVal*> getConstVals() { return this->const_vals;  }
		inline void pushVarName(VarName* vn) { this->var_names.push_back(vn);  }
		inline void pushConstVal(ConstVal* cv) { this->const_vals.push_back(cv);  }
		inline void pushToken(sp::Token* tok) { 
			//if (tok->getType() == sp::Token::TokenType::NAME) {
			//	VarName* vn = new VarName(tok, tok->getLiteral());
			//	pushVarName(vn);
			//}
			//else if (tok->getType() == sp::Token::TokenType::CONST) {
			//	ConstVal* cv = new ConstVal(tok, std::stoi(tok->getLiteral()));
			//	pushConstVal(cv);
			//}
			this->tokens.push_back(tok);
		}
		~CondExprBag() {
			for (int i = 0; i < var_names.size(); ++i) {
				delete var_names[i];
			}
			var_names.clear();
			for (int i = 0; i < const_vals.size(); ++i) {
				delete const_vals[i];
			}
			const_vals.clear();
			for (int i = 0; i < tokens.size(); ++i) {
				delete tokens[i];
			}
			const_vals.clear();
		}
	};
}
