#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"

// AST representing a Constant
namespace ast {
    class ConstVal : public Expr
    {
    public:
        const CONST val;

        ConstVal(sp::Token* token, CONST val) :Expr { token }, val{ val } {}

        // not ready
        bool compare(Expr* expr) override {
            return false;
        };

        STRING toString() override { return "(" + std::to_string(this->val) + ")"; };
        int getVal() { return val; }
    };
}
