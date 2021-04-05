#pragma once
#include <vector>
#include "SP/Token.h"
#include "AST/Node.h"
#include "AST/Stmt.h"
#include "AST/StmtLst.h"

// AST representing a Statement List
namespace ast {
    class StmtLst : public Node
    {
        std::vector<Stmt*> statements;
    public:
        StmtLst(sp::Token* token, std::vector<Stmt*> statements) :Node{ token }, statements{ statements} {}
        std::vector<Stmt*> getStatements() { return statements;  };
        STRING toString() {
            // not performant, only use debugging
            STRING out = "{\n";
            for (StmtNum i = 0; i < statements.size(); ++i) {
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
            for (StmtNum i = 0; i < statements.size(); ++i) {
                delete statements[i];
            }
            statements.clear();
        }
    };
}
