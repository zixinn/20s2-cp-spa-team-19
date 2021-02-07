#pragma once
// Procedure 
#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/ProcName.h"
#include "AST/StmtLst.h"

namespace ast {
	class Proc : public Node
	{

		ast::ProcName* proc_name;
		ast::StmtLst* stmt_lst;
	public:

		Proc(sp::Token* token, ast::ProcName* proc_name, ast::StmtLst* stmt_lst) 
			:Node{ token }, proc_name{ proc_name }, stmt_lst{ stmt_lst } {}		// contructor

		ast::ProcName* getName() { return proc_name; }
		ast::StmtLst* getStmtLst() { return stmt_lst; }
		~Proc() {};
	};
}
