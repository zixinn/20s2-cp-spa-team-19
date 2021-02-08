#pragma once

#include <vector>
#include <string>
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
		std::string toString() {
			// not performant, only use debugging
			std::string out = "{\n";
			for (int i = 0; i < statements.size(); ++i) {
				auto stmt = statements[i];
				if (!stmt) {
					throw "StmtLst toString: nullptr found";
				} 
				out += "    " + (*stmt).toString() + "\n";
			}
			out += "}\n";
			return out;
		}
		~StmtLst() {
			for (int i = 0; i < statements.size(); ++i) {
				delete statements[i];
			}
			statements.clear();
		}
	};
}
