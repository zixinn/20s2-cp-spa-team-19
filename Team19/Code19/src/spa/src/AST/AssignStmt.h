#pragma once
#include "SP/Token.h"
#include "AST/Expr.h"
#include "AST/VarName.h"
#include "AST/Stmt.h"

// Assign
namespace ast {
	class AssignStmt : public Stmt {
		VarName* var_name;
		Expr* expr;
	public:
	//	AssignStmt(int stmtNum, Token token, VarName* var_name, Expr* expr);
		AssignStmt(int stmtNum, sp::Token* token, VarName* var_name, Expr* expr)
			:Stmt( token, stmtNum ), var_name{ var_name }, expr{ expr } {}
		VarName* getName() { return var_name; }
		Expr* getExpr() { return expr; }
		bool compareExpr(Expr* otherExpr) { return this->expr->compare(otherExpr); }

		~AssignStmt() {
			delete var_name;
			delete expr;
		}

	};
}
