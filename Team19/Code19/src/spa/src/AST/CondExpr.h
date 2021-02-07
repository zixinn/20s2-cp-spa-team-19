#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/Node.h"

// Interface CondExpr for booleans
namespace ast {
	class CondExpr : public Node
	{
	public:
		CondExpr(sp::Token* token) :Node(token) {};
		virtual std::string toString() = 0;
		~CondExpr();
	};
}
