#pragma once
//  VarName
#include <typeinfo>
#include "SP/Token.h"
#include "AST/Expr.h"

namespace ast {
	class VarName : public Expr
	{
	public:
		const std::string val;

		VarName(sp::Token* token, std::string val) :Expr{ token }, val{ val } {}
		std::string toString() override { return this->val; };
		std::string getVal() { return val; }

		// DO NOT TRUST THIS, IF YOU ARE USING THIS METHOD LET ME KNOW
		bool compare(Expr* expr) override {
			if (!expr) { throw "compare against null"; }
			//if (!Expr::compareTokenType(expr)) { return false; }
			// PROBLEM: a proc_name and a var_name can have same name, maybe add new TokenType?
			return false; // this->getTokenLiteral() == expr->getTokenLiteral();
		};
	};
}
