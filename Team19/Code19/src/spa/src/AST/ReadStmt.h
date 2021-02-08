#pragma once
#include "AST/Stmt.h"
#include "SP/Token.h"

// Print
namespace ast {
	class ReadStmt : public Stmt {
		VarName* var_name;
	public:
		ReadStmt(int stmtNum, sp::Token* token, VarName* var_name)
			:Stmt( token, stmtNum ), var_name{ var_name } {}
		VarName* getName() { return var_name; }

		std::string toString() override {
			std::string var_str = var_name ? var_name->toString() : "[NULL_PTR]";
			return "read " + var_str + ";";
		}

		~ReadStmt() {
			delete var_name;
		}

	};
}
