#pragma once

#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/Stmt.h"

// Interface Stmt for anything with a statement number
namespace ast {
	class Stmt : public Node
	{
		int index;
	public:
		Stmt(sp::Token* token, int index) :Node{ token }, index{ index } {};
		int getIndex() { return index; };
	};
}
