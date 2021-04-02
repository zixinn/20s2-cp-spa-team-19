#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"

// AST representing a Prefix Conditional Expression
namespace ast {
    class PrefixCondExpr : public CondExpr {
        CondExpr* right;
    public:
        PrefixCondExpr(sp::Token* token, CondExpr* right)
                :CondExpr( token ), right{ right } {}
        CondExpr* getRight() { return right; }

        STRING toString() override {
            STRING right_str = right ? right->toString() : "[NULL_PTR]";
            //return this->getTokenLiteral() + "(" + right_str + ")";
            return this->getTokenLiteral() + "(" + right_str + ")";
        }

        ~PrefixCondExpr() {
            delete right;
        }

    };
}
