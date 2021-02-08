#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"
#include "AST/ProcName.h"

namespace ast {
	class ProcName : public Expr
	{
	public:
		const std::string val;

		ProcName(sp::Token* token, std::string val) :Expr { token }, val{ val } {}

		bool compare(Expr* expr) override {
			return false; // shouldnt have to compare procedure names
		};

		std::string toString() override {
			return this->val;
		};

		std::string getVal() { return val; }
	};
}
