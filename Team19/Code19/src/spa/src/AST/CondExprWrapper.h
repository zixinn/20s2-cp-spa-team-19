#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"

// Wrapper Class around a CondExpr for easier access to the Expression
namespace ast {
    class CondExprWrapper : public CondExpr {
        Expr* inside;
    public:
        CondExprWrapper(sp::Token* token, Expr* inside)
                :CondExpr(token), inside{ inside } {}
        Expr* unWrap() { return inside; }

        STRING toString() override {
            STRING inner_str = inside ? inside->toString() : "[NULL_PTR]";
            return inner_str;
        }

        ~CondExprWrapper() {
            delete inside;
        }

        static inline CondExprWrapper* wrap(Expr* expr) {
            return new CondExprWrapper(expr->getToken(), expr);
        }

    };
}
