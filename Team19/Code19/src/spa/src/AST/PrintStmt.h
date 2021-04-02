#pragma once
#include "AST/Stmt.h"
#include "SP/Token.h"

// AST representing a Print statement
namespace ast {
    class PrintStmt : public Stmt {
        VarName* var_name;
    public:
        PrintStmt(StmtNum stmtNum, sp::Token* token, VarName* var_name)
                :Stmt( token, stmtNum ), var_name{ var_name } {}
        VarName* getName() { return var_name; }

        STRING toString() override {
            STRING var_str = var_name ? var_name->toString() : "[NULL_PTR]";
            return "print " + var_str + ";";
        }

        ~PrintStmt() {
            delete var_name;
        }

    };
}
