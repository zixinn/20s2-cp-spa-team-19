#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"

// AST representing a Relational Conditional Expression
namespace ast {
    class RelExpr : public CondExpr {
        Expr* left;
        Expr* right;
    public:
        RelExpr(sp::Token* token, Expr* left, Expr* right)
                :CondExpr( token ), left{ left }, right{ right } {}
        Expr* getLeft() { return left; }
        Expr* getRight() { return right; }

        STRING toString() override {
            STRING left_str = left ? left->toString() : "[NULL_PTR]";
            STRING right_str = right ? right->toString() : "[NULL_PTR]";
            //return "(" + left_str + " " + this->getTokenLiteral() + " " + right_str + ")";
            return left_str + " " + this->getTokenLiteral() + " " + right_str;
        }

        ~RelExpr() {
            delete left;
            delete right;
        }

    };
}
