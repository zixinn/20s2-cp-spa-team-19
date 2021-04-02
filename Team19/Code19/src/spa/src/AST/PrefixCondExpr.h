#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/CondExpr.h"

namespace ast {
	class PrefixCondExpr : public CondExpr {
		CondExpr* right;
	public:
		PrefixCondExpr(sp::Token* token, CondExpr* right)
			:CondExpr( token ), right{ right } {}
		CondExpr* getRight() { return right; }

		std::string toString() override {
			std::string right_str = right ? right->toString() : "[NULL_PTR]";
			//return this->getTokenLiteral() + "(" + right_str + ")";
			return this->getTokenLiteral() + "(" + right_str + ")";
		}

		~PrefixCondExpr() {
			delete right;
		}

	};
}
