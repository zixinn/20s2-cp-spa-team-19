#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/CondExpr.h"

namespace ast {
	class RelExpr : public CondExpr {
		Expr* left;
		Expr* right;
	public:
		RelExpr(sp::Token* token, Expr* left, Expr* right)
			:CondExpr( token ), left{ left }, right{ right } {}
		Expr* getLeft() { return left; }
		Expr* getRight() { return right; }

		std::string toString() override {
			std::string left_str = left ? left->toString() : "[NULL_PTR]";
			std::string right_str = right ? right->toString() : "[NULL_PTR]";
			return "(" + left_str + " " + this->getTokenLiteral() + " " + right_str + ")";
		}

		~RelExpr() {
			delete left;
			delete right;
		}

	};
}
