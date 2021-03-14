#pragma once

#include "../AbstractAPI.h"
#include "../AST/Stmt.h"

class StmtTable {
public:

    // Constructor for StmtTable
    StmtTable();

    // Returns true if stmtNum is an If stmt and that it has that control variable
    bool isIfStmtWithControlVar(StmtNum stmtNum, ID controlVarID);

    // Returns true if stmtNum is an While stmt and that it has that control variable
    bool isWhileStmtWithControlVar(StmtNum stmtNum, ID controlVarID);

    // Returns the statement node given the stmtNum. Throws error if statement is not in statement table.
    ast::Stmt* getStmtNode(StmtNum stmtNum);

    // Returns the set of statement numbers given the control variable. Returns an empty set if it is not a control variable of an if(while) statement.
    unordered_set<StmtNum> const &getIfStmtsWithControlVar(ID controlVarID) const;
    unordered_set<StmtNum> const &getWhileStmtsWithControlVar(ID controlVarID) const;

    // Returns the control variable(s) given the statement number. Returns an empty set if the statement number given is not an if(while) statement / does not use a control variable.
    unordered_set<ID> const &getControlVarsOfIfStmt(StmtNum stmtNum) const;
    unordered_set<ID> const &getControlVarsOfWhileStmt(StmtNum stmtNum) const;

    // Returns the read/print variable given the stmtNum. Return -1 if the stmtNum is not a read(print) statement
    ID getReadVariableOfStmt(StmtNum stmtNum);
    ID getPrintVariableOfStmt(StmtNum stmtNum);

    // Returns the const reference to a set of read(print) statement numbers with the given variable.
    unordered_set<StmtNum> const &getStmtNumsOfReadWithVar(ID readVarID) const;
    unordered_set<StmtNum> const &getStmtNumsOfPrintWithVar(ID printVarID) const;

    // Returns a pair of vectors in the ifPatternsMap / whilePatternsMap.
    // First vector is a vector of stmtNums. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in the PatternsMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<StmtNum>, vector<ID> > getAllIfPatterns();
    pair<vector<StmtNum>, vector<ID> > getAllWhilePatterns();

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

    // Returns the number of entries in ifPatternsMap
    // E.g. if the ifPatternsMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getIfPatternsSize();

    // Returns the number of entries in whilePatternsMap
    // E.g. if the ifPatternsMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getWhilePatternsSize();

    // Stores <stmtNum, *AST> into stmtASTMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeStmt(StmtNum stmtNum, ast::Stmt *stmtNode, STRING type);

    // Stores <stmtNum, pair<lhs, rhs>> into assignExprMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeAssignExpr(StmtNum stmtNum, STRING varName, STRING expr);

    // Stores <stmtnum, varID> into read(print)VariablesMap
    // Stores <varID, set<stmtNum>> into the reverseRead(Print)VariablesMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeReadVariableForStmt(StmtNum stmtNum, ID readVarID);
    bool storePrintVariableForStmt(StmtNum stmtNum, ID printVarID);

    // Stores <stmtNum, controlVar> into ifPatternsMap, <controlVar, stmtNum> into reverseIfPatternsMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeIfPattern(StmtNum stmtNum, ID controlVarID);

    // Stores <stmtNum, controlVar> into whilePatternsMap, <controlVar, stmtNum> into reverseWhilePatternsMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeWhilePattern(StmtNum stmtNum, ID controlVarID);

    // Checks whether stmtASTMap contains stmtNum.
    bool hasStmt(StmtNum stmtNum);

private:
    // A map that stores the statement number as key and the StmtNode as value
    unordered_map<StmtNum, ast::Stmt*> stmtASTMap;

    // A map that stores the statement number as key and pair<lhs, rhs> of assignment statement as value
    unordered_map<StmtNum, pair<STRING, STRING> > assignExprMap;

    // A map that stores the statement number of read/print statements as key, the variable as value
    unordered_map<StmtNum, ID> readVariablesMap;
    unordered_map<StmtNum, ID> printVariablesMap;

    unordered_map<ID, unordered_set<StmtNum> > reverseReadVariablesMap;
    unordered_map<ID, unordered_set<StmtNum> > reversePrintVariablesMap;

    // Stores StmtNum as key, and the set of control variable as value
    unordered_map<StmtNum, unordered_set<ID> > ifPatternsMap;
    unordered_map<StmtNum, unordered_set<ID> > whilePatternsMap;

    // Stores control variable as key, and a set of stmtNum as value
    unordered_map<ID, unordered_set<StmtNum> > reverseIfPatternsMap;
    unordered_map<ID, unordered_set<StmtNum> > reverseWhilePatternsMap;

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


