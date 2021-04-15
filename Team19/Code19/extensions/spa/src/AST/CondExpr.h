#pragma once
#include "SP/Token.h"
#include "AST/Node.h"

// Interface CondExpr for booleans
namespace ast {
    class CondExpr : public Node
    {
    public:
        CondExpr(sp::Token* token) :Node(token) {};
        virtual STRING toString() = 0;
        ~CondExpr() {};
    };
}
