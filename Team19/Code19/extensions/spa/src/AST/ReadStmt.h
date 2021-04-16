#pragma once
#include "AST/Stmt.h"
#include "SP/Token.h"

// AST representing a Read Statement
namespace ast {
    class ReadStmt : public Stmt {
        VarName* var_name;
    public:
        ReadStmt(StmtNum stmtNum, sp::Token* token, VarName* var_name)
                :Stmt( token, stmtNum ), var_name{ var_name } {}
        VarName* getName() { return var_name; }

        STRING toString() override {
            STRING var_str = var_name ? var_name->toString() : "[NULL_PTR]";
            return "read " + var_str + ";";
        }

        ~ReadStmt() {
            delete var_name;
        }

    };
}
