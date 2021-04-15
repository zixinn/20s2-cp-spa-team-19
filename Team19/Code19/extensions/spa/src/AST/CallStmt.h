#pragma once
#include "SP/Token.h"
#include "AST/ProcName.h"
#include "AST/Stmt.h"

// AST representing a Call Statement
namespace ast {
    class CallStmt : public Stmt {
        ProcName* proc_name;
    public:
        CallStmt(StmtNum stmtNum, sp::Token* token, ProcName* proc_name)
                :Stmt( token, stmtNum ), proc_name{ proc_name } {}
        ProcName* getName() { return proc_name; }

        STRING toString() override {
            STRING proc_str = proc_name ? proc_name->toString() : "[NULL_PTR]";
            return "call " + proc_str + ";";
        }

        ~CallStmt() {
            delete proc_name;
        }

    };
}
