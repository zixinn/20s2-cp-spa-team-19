#pragma once
#include "SP/Token.h"
#include "AST/ProcName.h"
#include "AST/Stmt.h"

// Call
namespace ast {
	class CallStmt : public Stmt {
		ProcName* proc_name;
	public:
		CallStmt(int stmtNum, sp::Token* token, ProcName* proc_name)
			:Stmt( token, stmtNum ), proc_name{ proc_name } {}
		ProcName* getName() { return proc_name; }

		std::string toString() override {
			std::string proc_str = proc_name ? proc_name->toString() : "[NULL_PTR]";
			return "call " + proc_str + ";";
		}

		~CallStmt() {
			delete proc_name;
		}

	};
}
