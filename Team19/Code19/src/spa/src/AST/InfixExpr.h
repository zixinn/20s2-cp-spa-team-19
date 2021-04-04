#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"

// AST representing an Infix Expression
namespace ast {
    class InfixExpr : public Expr {
        Expr* left;
        Expr* right;
    public:
        InfixExpr(sp::Token* token, Expr* left, Expr* right)
                :Expr( token ), left{ left }, right{ right } {}
        Expr* getLeft() { return left; }
        Expr* getRight() { return right; }
        bool compare(Expr* otherExpr) override { throw "NOT READY"; }

        STRING toString() override {
            STRING left_str = left ? left->toString() : "[NULL_PTR]";
            STRING right_str = right ? right->toString() : "[NULL_PTR]";
            return "(" + left_str + " " + this->getTokenLiteral() + " " + right_str + ")";
        }

        ~InfixExpr() {
            delete left;
            delete right;
        }

    };
}
