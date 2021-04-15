#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/StmtLst.h"
#include "AST/Stmt.h"

// AST representing a While Statement
namespace ast {
    class WhileStmt : public Stmt {
        CondExpr* cond_expr;
        StmtLst* stmt_lst;
    public:
        WhileStmt(StmtNum stmtNum, sp::Token* token, CondExpr* cond_expr, StmtLst* stmt_lst)
                :Stmt(token, stmtNum), cond_expr{ cond_expr }, stmt_lst{ stmt_lst } {}
        CondExpr* getCondExpr() { return cond_expr; }
        StmtLst* getStmtLst() { return stmt_lst; }

        STRING toString() override {
            STRING cond_str = cond_expr ? cond_expr->toString() : "[NULL_PTR]";
            STRING sl_str = stmt_lst ? stmt_lst->toString() : "[NULL_PTR]";
            return "while (" + cond_str + ") " + sl_str;
        }

        ~WhileStmt() {
            delete cond_expr;
            delete stmt_lst;
        }

    };
}
