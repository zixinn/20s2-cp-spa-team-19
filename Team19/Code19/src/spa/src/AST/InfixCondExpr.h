#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"

// AST representing an Infix Conditional Statement. It has 2 children, where both can be CondExprs.
namespace ast {
    class InfixCondExpr : public CondExpr {
        CondExpr* left;
        CondExpr* right;
    public:
        InfixCondExpr(sp::Token* token, CondExpr* left, CondExpr* right)
                :CondExpr( token ), left{ left }, right{ right } {}
        CondExpr* getLeft() { return left; }
        CondExpr* getRight() { return right; }

        STRING toString() override {
            STRING left_str = left ? left->toString() : "[NULL_PTR]";
            STRING right_str = right ? right->toString() : "[NULL_PTR]";
            return "(" + left_str + ") " + this->getTokenLiteral() + " (" + right_str + ")";
        }

        ~InfixCondExpr() {
            delete left;
            delete right;
        }

    };
}
