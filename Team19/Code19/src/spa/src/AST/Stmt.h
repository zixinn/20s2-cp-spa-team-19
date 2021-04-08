#pragma once
#include "AST/Node.h"

// Interface Stmt for anything with a statement number
namespace ast {
    class Stmt : public Node
    {
        StmtNum index;
    public:
        Stmt(sp::Token* token, StmtNum index) :Node{ token }, index{ index } {};
        StmtNum getIndex() { return index; };
        virtual STRING toString() { return "STMT: OVERRIDE THIS"; };
        ~Stmt() {};
    };
}
