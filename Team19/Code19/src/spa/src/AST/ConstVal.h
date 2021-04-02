#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"

namespace ast {
	class ConstVal : public Expr
	{
	public:
		const int val;

		ConstVal(sp::Token* token, int val) :Expr { token }, val{ val } {}

		// not ready
		bool compare(Expr* expr) override {
			return false; 
		};

		std::string toString() override { return "(" + std::to_string(this->val) + ")"; };
		int getVal() { return val; }
	};
}
