#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/CondExpr.h"

// Assign
namespace ast {
	class InfixCondExpr : public CondExpr {
		CondExpr* left;
		CondExpr* right;
	public:
		InfixCondExpr(sp::Token* token, CondExpr* left, CondExpr* right)
			:CondExpr( token ), left{ left }, right{ right } {}
		CondExpr* getLeft() { return left; }
		CondExpr* getRight() { return right; }

		std::string toString() override {
			std::string left_str = left ? left->toString() : "[NULL_PTR]";
			std::string right_str = right ? right->toString() : "[NULL_PTR]";
			return "(" + left_str + ") " + this->getTokenLiteral() + " (" + right_str + ")";
		}

		~InfixCondExpr() {
			delete left;
			delete right;
		}

	};
}
