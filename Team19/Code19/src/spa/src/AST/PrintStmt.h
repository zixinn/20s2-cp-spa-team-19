#pragma once
#include "AST/Stmt.h"
#include "SP/Token.h"

// Print
namespace ast {
	class PrintStmt : public Stmt {
		VarName* var_name;
	public:
		PrintStmt(int stmtNum, sp::Token* token, VarName* var_name)
			:Stmt( token, stmtNum ), var_name{ var_name } {}
		VarName* getName() { return var_name; }

		~PrintStmt() {
			delete var_name;
		}

	};
}
