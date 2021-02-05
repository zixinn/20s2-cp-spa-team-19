#pragma once

#include "../AbstractAPI.h"

class Modifies {
public:
    // Constructor for Modifies
    Modifies();

    // Returns TRUE if Modifies(stmtNum, variable) holds and the information is stored in the PKB, returns FALSE otherwise.
    bool stmtModifiesVar(StmtNum stmtNum, ID varID);

    // Returns TRUE if Modifies(proc, variable) holds and the information is stored in the PKB, returns FALSE otherwise.
    bool procModifiesVar(ID procID, ID varID);

    // returns the const reference to unordered set of variables modified by the stmtNum.
    unordered_set<ID> const &getVarsModifiedByStmt(StmtNum stmtNum) const;

    // returns the const reference to unordered set of variables modified by the procID.
    unordered_set<ID> const &getVarsModifiedByProc(ID procID) const;

    // returns the const reference to unordered set of statements that modifies the varID.
    unordered_set<StmtNum> const &getStmtsModifies(ID varID) const;

    // returns the const reference to unordered set of procIDs that modifies varID.
    unordered_set<StmtNum> const &getProcsModifies(ID varID) const;

    // Returns a pair of vectors in the stmtModifiesMap.
    // First vector is a vector of statements. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in stmtModifiesMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<StmtNum>, vector<ID> > getAllStmtModifies();

    // Returns a pair of vectors in the procModifiesMap.
    // First vector is a vector of procIDs. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in procModifiesMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<ID>, vector<ID> > getAllProcModifies();

    // Returns the number of Modifies relationship in stmtModifiesMap.
    // If stmtModifiesMap has [1: {2,3}], then the size is 2.
    int getStmtSize();

    // Returns the number of Modifies relationship in procModifiesMap.
    // If procModifiesMap has [1: {2,3}], then the size is 2.
    int getProcSize();

    // add <stmtNum, variableID> to the StmtModifiesMap.
    // Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeStmtModifies(StmtNum stmtNum, ID varID);

    // add <procedureID, variableIs> to the StmtModifiesMap.
    // Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeProcModifies(ID procID, ID varID);

private:

    // Stores StmtNum as key, and the set of variables that is modified be the StmtNum as values
    unordered_map<StmtNum, unordered_set<ID> > stmtModifiesMap;
    // Stores VarID as key, the set of StmtNums that modifies the variable as values
    unordered_map<ID, unordered_set<StmtNum> > reverseStmtModifiesMap;
    // Stores ProcID as key, and the set of variables that is modified be the StmtNum as values
    unordered_map<ID, unordered_set<ID> > procModifiesMap;
    // Stores VarID as key, the set of Procedures that modifies the variable as values
    unordered_map<ID, unordered_set<ID> > reverseProcModifiesMap;
};
