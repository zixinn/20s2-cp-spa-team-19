#pragma once
#include "Index.fwd.h"

//#include "Node.fwd.h"
//#include "Expr.fwd.h"

// Interface Expr for RHS of Assignments
namespace ast {
	class Expr : public Node
	{
	public:
		Expr(Token* token) :Node{ token } {};
		virtual bool compare(Expr* expr) = 0;
		virtual std::string toString() = 0;
	};
}
