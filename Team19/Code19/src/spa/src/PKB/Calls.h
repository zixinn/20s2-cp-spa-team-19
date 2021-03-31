#pragma once

#include "../AbstractAPI.h"

class Calls {
public:
    // Constructor for CallsTable
    Calls();

    // Returns true if the program has cyclical calls
    bool hasCyclicalCall();

    // Returns true if Calls(p,q)
    bool isCalls(ID p, ID q);

    // Returns true if Calls*(p,q)
    bool isCallsStar(ID p, ID q);

    // Returns the callee of the call statement that occurs at stmtNum. Returns -1 if statement is not a call statement.
    ID getCalleeInStmt(StmtNum stmtNum);

    // Returns the const reference to a set of call statements that calls callee.
    // Return an empty set if there is no such callee
    unordered_set<StmtNum> const &getStmtNumThatCallsCallee(ID calleeID) const;

    // Returns a const reference to the set of p's such that Calls(p,q)
    unordered_set<ID> const &getCallers(ID q) const;

    // Returns a const reference to the set of q's such that Calls(p,q)
    unordered_set<ID> const &getCallees(ID p) const;

    // Returns a const reference to the set of p's such that Calls*(p,q)
    unordered_set<ID> const &getCallersStar(ID q) const;

    // Returns a const reference to the set of q's such that Calls*(p,q)
    unordered_set<ID> const &getCalleesStar(ID p) const;

    // Returns a const reference to the set of stmts where Calls(p,q) occur
    unordered_set<StmtNum> const &getStmtsOfCalls(ID p, ID q);

    // Returns a pair of vectors in the callsMap.
    // First vector is vector of p's. Second is vector of q's.
    // e.g. if (1,2), (3,4), (5,6) is in callsMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ID>, vector<ID> > getAllCalls();

    // Returns a pair of vectors in the callsStarMap.
    // First vector is vector of p's. Second is vector of q's.
    // e.g. if (1,2), (3,4), (5,6) is in callsStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ID>, vector<ID> > getAllCallsStar();

    // Returns the no. of pairs of Calls relationship.
    // E.g. if the callsMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getCallsSize();

    // Returns the no. of pairs of Calls* relationship.
    // E.g. if the callsStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getCallsStarSize();

    // Stores a calls relationship into the callsRawInfoTable. Returns true if the information is successfully added to the PKB.
    bool storeCalls(StmtNum stmtNum, ID p, ID q);

    // Returns true if the calls statement at stmtNum is already stored in the callsRawInfoTable.
    bool hasCalls(StmtNum stmtNum);

    // populate the rest of the call tables and handles Modifies and Uses.
    // Method called after all the calls have been stored in callsRawInfoTable.
    // Returns true if the processing is successful and there are no cyclical calls detected.
    bool processCalls();

private:

    // Keeps track of whether the program has cyclical procedure calls
    bool isCyclic{};

    // The first table to be populated in storeCall(). Info stored will be used to populate the rest of the tables/maps.
    unordered_map<StmtNum, pair<ID, ID> > callsRawInfoTable;

    // Stores p as key, an unordered map (q as key, stmtNum as value) as value
    unordered_map<ID, unordered_map<ID, unordered_set<StmtNum> > > callsStmtMap;

    // Stores StmtNum where Calls(p,q) occurs as key, q as value.
    unordered_map<StmtNum, ID> callsStmtCalleeMap;

    // Store q as key, the set of statements that calls q as value.
    unordered_map<ID, unordered_set<StmtNum> > reverseCallsStmtCalleeMap;

    // Stores p as key, q as value for each Calls(p,q).
    unordered_map<ID, unordered_set<ID>> callsMap;

    // Stores q as key, p as value for each Calls(p,q).
    unordered_map<ID, unordered_set<ID>> reverseCallsMap;

    // Stores p as key, q as value for each Calls*(p,q).
    unordered_map<ID, unordered_set<ID>> callsStarMap;

    // Stores q as key, p as value for each Calls*(p,q).
    unordered_map<ID, unordered_set<ID>> reverseCallsStarMap;

    // To keep track of the procedures that we have processed in DFS
    unordered_set<ID> processedProcedures;

    // Stores <p, q> in the callsStarMap.
    // Stores <q, p> in reverseCallsStarMap
    // Returns true if info is successfully added.
    bool storeCallStar(ID p, ID q);

    // calculate all Calls* relationships from the callsMap using BFS and populate the callsStarMap and reverseCallsStarMap
    void populateCallsStar();

    // For each callers p that we have, find out who their direct callees q's are
    // For each callee procedure q, process q first (DFS with implicit stack)
    // Find the stmtNums where the Calls(p, q) occurs
    // For each stmtNum, storeUsesModifies(stmtNum, p, q)
    void updateAllUsesAndModifies();

    void updateUsesAndModifiesForProcedure(ID p);

    // Stores Uses and Modifies relationship for procedure p and statement StmtNum, and container statements within procedure p,
    // according to the Uses and Modifies relationship of q.
    void storeUsesAndModifies(StmtNum stmtNum, ID p, ID q);

};

