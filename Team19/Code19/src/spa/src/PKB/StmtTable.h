#pragma once

#include "../AbstractAPI.h"
#include "../AST/Stmt.h"

class StmtTable {
public:

    // Constructor for StmtTable
    StmtTable();

    // Returns the statement node given the stmtNum. Throws error if statement is not in statement table.
    ast::Stmt* getStmtNode(StmtNum stmtNum);

    // Returns the pair<lhs, rhs> of assignment given the stmtNum. Throws error if statement is not in statement table.
    pair<STRING, STRING> getAssignExpr(StmtNum stmtNum);

    // Returns const reference to the vector of all statement numbers of a particular type
    vector<StmtNum> const &getAllAssignStmtNums() const;
    vector<StmtNum> const &getAllReadStmtNums() const;
    vector<StmtNum> const &getAllPrintStmtNums() const;
    vector<StmtNum> const &getAllCallStmtNums() const;
    vector<StmtNum> const &getAllWhileStmtNums() const;
    vector<StmtNum> const &getAllIfStmtNums() const;
    vector<StmtNum> const &getAllStmtNums() const;

    // Returns the number of statements stored
    int getSize();

    // Stores <stmtNum, *AST> into stmtASTMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeStmt(StmtNum stmtNum, ast::Stmt *stmtNode, STRING type);

    // Stores <stmtNum, pair<lhs, rhs>> into assignExprMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeAssignExpr(StmtNum stmtNum, STRING varName, STRING expr);

    // Checks whether stmtASTMap contains stmtNum.
    bool hasStmt(StmtNum stmtNum);

private:
    // A map that stores the statement number as key and the StmtNode as value
    unordered_map<StmtNum, ast::Stmt*> stmtASTMap;

    // A map that stores the statement number as key and pair<lhs, rhs> of assignment statement as value
    unordered_map<StmtNum, pair<STRING, STRING>> assignExprMap;

    // Vectors for each type of statement
    // Stores corresponding statement numbers of statements with that type
    vector<StmtNum> stmtNums;
    vector<StmtNum> assignStmtNums;
    vector<StmtNum> readStmtNums;
    vector<StmtNum> printStmtNums;
    vector<StmtNum> callStmtNums;
    vector<StmtNum> whileStmtNums;
    vector<StmtNum> ifStmtNums;

};


