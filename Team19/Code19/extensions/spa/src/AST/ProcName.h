#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"

// AST representing a Procedure Name
namespace ast {
    class ProcName : public Expr
    {
    public:
        const STRING val;

        ProcName(sp::Token* token, STRING val) :Expr { token }, val{ val } {}

        bool compare(Expr* expr) override {
            return false; // shouldnt have to compare procedure names
        };

        STRING toString() override {
            return this->val;
        };

        STRING getVal() { return val; }
        ~ProcName() {};
    };
}
