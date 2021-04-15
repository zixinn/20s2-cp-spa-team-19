#pragma once

#include <set>

#include "../AbstractAPI.h"
#include "../AST/Stmt.h"

class StmtTable {
public:
    // Constructor for StmtTable
    StmtTable();

    // Returns true if stmtNum is an If stmt and that it has that control variable
    bool isIfStmtWithControlVar(StmtNum stmtNum, VarID controlVarID);

    // Returns true if stmtNum is an While stmt and that it has that control variable
    bool isWhileStmtWithControlVar(StmtNum stmtNum, VarID controlVarID);

    // Returns the statement node given the stmtNum. Throws error if statement is not in statement table.
    ast::Stmt* getStmtNode(StmtNum stmtNum);

    // Returns the set of statement numbers given the control variable. Returns an empty set if it is not a control variable of an if(while) statement.
    unordered_set<StmtNum> const &getIfStmtsWithControlVar(VarID controlVarID) const;
    unordered_set<StmtNum> const &getWhileStmtsWithControlVar(VarID controlVarID) const;

    // Returns the control variable(s) given the statement number. Returns an empty set if the statement number given is not an if(while) statement / does not use a control variable.
    const unordered_set<VarID> & getControlVarsOfIfStmt(StmtNum stmtNum) const;
    const unordered_set<VarID> & getControlVarsOfWhileStmt(StmtNum stmtNum) const;

    // Returns the read/print variable given the stmtNum. Return -1 if the stmtNum is not a read(print) statement
    VarID getReadVariableOfStmt(StmtNum stmtNum);
    VarID getPrintVariableOfStmt(StmtNum stmtNum);

    // Returns the const reference to a set of read(print) statement numbers with the given variable.
    unordered_set<StmtNum> const &getStmtNumsOfReadWithVar(VarID readVarID) const;
    unordered_set<StmtNum> const &getStmtNumsOfPrintWithVar(VarID printVarID) const;

    // Returns a pair of vectors in the ifPatternsMap / whilePatternsMap.
    // First vector is a vector of stmtNums. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in the PatternsMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<StmtNum>, vector<VarID>> getAllIfPatterns();
    pair<vector<StmtNum>, vector<VarID>> getAllWhilePatterns();

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

    // Returns the if/else stmt's startStmt and endStmt if it exists. Throws exception otherwise.
    pair<StmtNum, set<StmtNum>> getIfStmtRange(StmtNum stmtNum);
    pair<StmtNum, set<StmtNum>> getElseStmtRange(StmtNum stmtNum);

    // Stores <stmtNum, *AST> into stmtASTMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeStmt(StmtNum stmtNum, ast::Stmt *stmtNode, STRING type);

    // Stores <stmtNum, pair<lhs, rhs>> into assignExprMap.
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeAssignExpr(StmtNum stmtNum, STRING varName, STRING expr);

    // Stores <stmtnum, varID> into read(print)VariablesMap
    // Stores <varID, set<stmtNum>> into the reverseRead(Print)VariablesMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeReadVariableForStmt(StmtNum stmtNum, VarID readVarID);
    bool storePrintVariableForStmt(StmtNum stmtNum, VarID printVarID);

    // Stores <stmtNum, controlVar> into ifPatternsMap, <controlVar, stmtNum> into reverseIfPatternsMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeIfPattern(StmtNum stmtNum, VarID controlVarID);

    // Stores <stmtNum, controlVar> into whilePatternsMap, <controlVar, stmtNum> into reverseWhilePatternsMap
    // Called by DE. Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeWhilePattern(StmtNum stmtNum, VarID controlVarID);

    // Stores the if/else stmt that spans from startStmt to endStmt. Returns true if insertion took place.
    bool storeIfStmt(StmtNum ifStmtNum, StmtNum startStmt, const set<StmtNum>& endStmt);
    bool storeElseStmt(StmtNum elseStmtNum, StmtNum startStmt, const set<StmtNum>& endStmt);

    // Checks whether stmtASTMap contains stmtNum.
    bool hasStmt(StmtNum stmtNum);

private:
    // A map that stores the statement number as key and the StmtNode as value
    unordered_map<StmtNum, ast::Stmt*> stmtASTMap;

    // A map that stores the statement number as key and pair<lhs, rhs> of assignment statement as value
    unordered_map<StmtNum, pair<STRING, STRING> > assignExprMap;

    // A map that stores the statement number of read/print statements as key, the variable as value
    unordered_map<StmtNum, VarID> readVariablesMap;
    unordered_map<StmtNum, VarID> printVariablesMap;

    // A map that stores the variableID of read/print statements as key, the set of statement numbers of these read/print statements as value
    unordered_map<VarID, unordered_set<StmtNum> > reverseReadVariablesMap;
    unordered_map<VarID, unordered_set<StmtNum> > reversePrintVariablesMap;

    // Stores StmtNum as key, and the set of control variable as value
    unordered_map<StmtNum, unordered_set<VarID> > ifPatternsMap;
    unordered_map<StmtNum, unordered_set<VarID> > whilePatternsMap;

    // Stores control variable as key, and a set of stmtNum as value
    unordered_map<VarID, unordered_set<StmtNum> > reverseIfPatternsMap;
    unordered_map<VarID, unordered_set<StmtNum> > reverseWhilePatternsMap;

    // A map that stores if stmt num as key, and a set of <startStmt, endStmt> as value.
    unordered_map<StmtNum, pair<StmtNum, set<StmtNum>>> ifStmtMap;
    unordered_map<StmtNum, pair<StmtNum, set<StmtNum>>> elseStmtMap;

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


