#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"
#include "AST/VarName.h"
#include "AST/Stmt.h"

// AST representing an Assign Statement
namespace ast {
	class AssignStmt : public Stmt {
		VarName* var_name;
		Expr* expr;
	public:
		AssignStmt(StmtNum stmtNum, sp::Token* token, VarName* var_name, Expr* expr)
			:Stmt( token, stmtNum ), var_name{ var_name }, expr{ expr } {}
		VarName* getName() { return var_name; }
		Expr* getExpr() { return expr; }
		bool compareExpr(Expr* otherExpr) { return this->expr->compare(otherExpr); }

		STRING toString() override {
			STRING var_str = var_name ? var_name->toString() : "[NULL_PTR]";
			STRING expr_str = expr ? expr->toString() : "[NULL_PTR]";
			return var_str + " = " + expr_str + ";";
		}

		~AssignStmt() {
			delete var_name;
			delete expr;
		}

	};
}
