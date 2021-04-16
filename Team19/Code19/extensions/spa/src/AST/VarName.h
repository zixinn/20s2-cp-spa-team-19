#pragma once
#include <typeinfo>
#include "SP/Token.h"
#include "AST/Expr.h"

// AST representing a Variable Name
namespace ast {
    class VarName : public Expr
    {
    public:
        const STRING val;

        VarName(sp::Token* token, STRING val) :Expr{ token }, val{ val } {}
        STRING toString() override { return "(" + this->val + ")"; };
        STRING getVal() { return val; }

        // DO NOT TRUST THIS, IF YOU ARE USING THIS METHOD LET ME KNOW
        bool compare(Expr* expr) override {
            if (!expr) { throw "compare against null"; }
            //if (!Expr::compareTokenType(expr)) { return false; }
            // PROBLEM: a proc_name and a var_name can have same name, maybe add new TokenType?
            return false; // this->getTokenLiteral() == expr->getTokenLiteral();
        };
        ~VarName() {};
    };
}
