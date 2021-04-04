#pragma once
#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/Expr.h"

// Interface Expr for the RHS of Assignments
// Expr is the parent of VarName, ConstVal and InfixExpr.
namespace ast {
    class Expr : public Node
    {
    public:
        Expr(sp::Token* token) :Node{ token } {};
        virtual bool compare(Expr* expr) = 0;
        virtual STRING toString() = 0;
        ~Expr() {};
    };
}
