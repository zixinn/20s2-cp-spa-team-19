#pragma once

#include "../AbstractAPI.h"

class Uses {
public:
    // Constructor for Uses
    Uses();

    // Returns TRUE if Uses(stmtNum, variable) holds and the information is stored in the PKB, returns FALSE otherwise.
    bool stmtUsesVar(StmtNum stmtNum, ID varID);

    // Returns TRUE if Uses(proc, variable) holds and the information is stored in the PKB, returns FALSE otherwise.
    bool procUsesVar(ID procID, ID varID);

    // returns the const reference to unordered set of variables used by the stmtNum.
    unordered_set<ID> const &getVarsUsedByStmt(StmtNum stmtNum) const;

    // returns the const reference to unordered set of variables used by the procID.
    unordered_set<ID> const &getVarsUsedByProc(ID procID) const;

    // returns the const reference to unordered set of statements that uses the varID.
    unordered_set<StmtNum> const &getStmtsUses(ID varID) const;

    // returns the const reference to unordered set of procIDs that uses varID.
    unordered_set<StmtNum> const &getProcsUses(ID varID) const;

    // Returns a pair of vectors in the stmtUsesMap.
    // First vector is a vector of statements. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in stmtUsesMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<StmtNum>, vector<ID> > getAllStmtUses();

    // Returns a pair of vectors in the procUsesMap.
    // First vector is a vector of procIDs. Second is a vector of varIDs.
    // For e.g., if (1,2), (3,4), (5,6) is in procUsesMap, then it will return <[1,3,5],[2,4,6]>
    pair<vector<ID>, vector<ID> > getAllProcUses();

    // Returns the number of Uses relationship in stmtUsesMap.
    // If stmtUsesMap has [1: {2,3}], then the size is 2.
    int getStmtSize();

    // Returns the number of Uses relationship in procUsesMap.
    // If procUsesMap has [1: {2,3}], then the size is 2.
    int getProcSize();

    // add <stmtNum, variableID> to the StmtUsesMap.
    // Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeStmtUses(StmtNum stmtNum, ID varID);

    // add <procedureID, variableIs> to the StmtUsesMap.
    // Returns TRUE if the information is added successfully, returns FALSE otherwise.
    bool storeProcUses(ID procID, ID varID);

private:

    // Stores StmtNum as key, and the set of variables that is used be the StmtNum as values
    unordered_map<StmtNum, unordered_set<ID> > stmtUsesMap;
    // Stores VarID as key, the set of StmtNums that uses the variable as values
    unordered_map<ID, unordered_set<StmtNum> > reverseStmtUsesMap;
    // Stores ProcID as key, and the set of variables that is used be the StmtNum as values
    unordered_map<ID, unordered_set<ID> > procUsesMap;
    // Stores VarID as key, the set of Procedures that uses the variable as values
    unordered_map<ID, unordered_set<ID> > reverseProcUsesMap;
};
