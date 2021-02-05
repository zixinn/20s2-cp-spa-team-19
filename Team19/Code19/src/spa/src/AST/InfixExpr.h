#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/Expr.h"

// Assign
namespace ast {
	class InfixExpr : public Expr {
		Expr* left;
		Expr* right;
	public:
		InfixExpr(sp::Token* token, Expr* left, Expr* right)
			:Expr( token ), left{ left }, right{ right } {}
		Expr* getLeft() { return left; }
		Expr* getRight() { return right; }
		bool compareExpr(Expr* otherExpr) { throw "NOT READY"; }

		std::string toString() override {
			std::string left_str = left ? left->toString() : "[NULL_PTR]";
			std::string right_str = right ? right->toString() : "[NULL_PTR]";
			return "(" + left_str + " " + this->getTokenLiteral() + " " + right_str + ")";
		}

		~InfixExpr() {
			delete left;
			delete right;
		}

	};
}
