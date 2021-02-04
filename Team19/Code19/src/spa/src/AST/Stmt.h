#pragma once
#include "AST/Node.h"

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
