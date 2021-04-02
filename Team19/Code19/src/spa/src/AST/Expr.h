#pragma once
#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/Expr.h"

// Interface Expr for RHS of Assignments
namespace ast {
	class Expr : public Node
	{
	public:
		Expr(sp::Token* token) :Node{ token } {};
		virtual bool compare(Expr* expr) = 0;
		virtual std::string toString() = 0;
		~Expr() {};
	};
}
