#pragma once
#include <string>
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

		std::string toString() override {
			std::string var_str = var_name ? var_name->toString() : "[NULL_PTR]";
			std::string expr_str = expr ? expr->toString() : "[NULL_PTR]";
			return var_str + " = " + expr_str + ";";
		}

		~AssignStmt() {
			delete var_name;
			delete expr;
		}

	};
}
