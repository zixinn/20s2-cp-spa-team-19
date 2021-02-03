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

		~CallStmt() {
			delete proc_name;
		}

	};
}
