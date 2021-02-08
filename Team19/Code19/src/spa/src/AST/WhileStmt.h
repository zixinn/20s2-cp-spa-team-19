#pragma once
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/StmtLst.h"
#include "AST/Stmt.h"

namespace ast {
	class WhileStmt : public Stmt {
		CondExpr* cond_expr;
		StmtLst* stmt_lst;
	public:
		WhileStmt(int stmtNum, sp::Token* token, CondExpr* cond_expr, StmtLst* stmt_lst)
			:Stmt(token, stmtNum), cond_expr{ cond_expr }, stmt_lst{ stmt_lst } {}
		CondExpr* getCondExpr() { return cond_expr; }

		std::string toString() override {
			std::string cond_str = cond_expr ? cond_expr->toString() : "[NULL_PTR]";
			std::string sl_str = stmt_lst ? stmt_lst->toString() : "[NULL_PTR]";
			return "while (" + cond_str + ") " + sl_str;
		}

		~WhileStmt() {
			delete cond_expr;
			delete stmt_lst;
		}

	};
}
