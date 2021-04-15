#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/StmtLst.h"
#include "AST/Stmt.h"

// AST representing an If Statement
namespace ast {
    class IfStmt : public Stmt {
        CondExpr* cond_expr;
        StmtLst* consequence;
        StmtLst* alternative;
    public:
        IfStmt(StmtNum stmtNum, sp::Token* token, CondExpr* cond_expr, StmtLst* consequence, StmtLst* alternative)
                :Stmt(token, stmtNum), cond_expr{ cond_expr }, consequence{ consequence }, alternative{ alternative } {}
        CondExpr* getCondExpr() { return cond_expr; }
        StmtLst* getConsequence() { return consequence; }
        StmtLst* getAlternative() { return alternative; }

        STRING toString() override {
            STRING cond_str = cond_expr ? cond_expr->toString() : "[NULL_PTR]";
            STRING csq_str = consequence ? consequence->toString() : "[NULL_PTR]";
            STRING alt_str = alternative ? alternative->toString() : "[NULL_PTR]";
            return "if (" + cond_str + ") then " + csq_str + "else " + alt_str;
        }

        ~IfStmt() {
            delete cond_expr;
            delete consequence;
            delete alternative;
        }

    };
}
