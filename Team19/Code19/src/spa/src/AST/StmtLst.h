#pragma once

#include <vector>
#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/Stmt.h"
#include "AST/StmtLst.h"


namespace ast {
	class StmtLst : public Node
	{
		std::vector<Stmt*> statements;
	public:
		StmtLst(sp::Token* token, std::vector<Stmt*> statements) :Node{ token }, statements{ statements} {}
		std::vector<Stmt*> getStatements() { return statements;  };
		~StmtLst() {
			for (int i = 0; i < statements.size(); ++i) {
				delete statements[i];
			}
			statements.clear();
		}
	};
}
