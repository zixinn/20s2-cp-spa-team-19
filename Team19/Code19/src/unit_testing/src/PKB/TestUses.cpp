#include "PKB/Uses.h"

#include "catch.hpp"
using namespace std;

Uses* setUpStmtUses() {
    Uses* uses = new Uses();
    uses->storeStmtUses(1,1);
    uses->storeStmtUses(1,2);
    uses->storeStmtUses(1,3);
    uses->storeStmtUses(2,1);
    uses->storeStmtUses(2,3);
    uses->storeStmtUses(3,1);
    return uses;
}

Uses* setUpProcUses() {
    Uses* uses = new Uses();
    uses->storeProcUses(1,4);
    uses->storeProcUses(1,2);
    uses->storeProcUses(1,3);
    uses->storeProcUses(2,1);
    uses->storeProcUses(2,3);
    uses->storeProcUses(3,4);
    return uses;
}

TEST_CASE("storeStmtUses Test") {
    Uses* uses = new Uses();
    REQUIRE(uses->storeStmtUses(1,1));
    REQUIRE(uses->storeStmtUses(1,2));
    REQUIRE(uses->storeStmtUses(1,3));
    REQUIRE(uses->storeStmtUses(2,1));
    REQUIRE(uses->storeStmtUses(2,3));
    REQUIRE(uses->storeStmtUses(3,1));
    REQUIRE_FALSE(uses->storeStmtUses(1,1));
    REQUIRE_FALSE(uses->storeStmtUses(3,1));
}

TEST_CASE("storeProcUses Test") {
    Uses* uses = new Uses();
    REQUIRE(uses->storeProcUses(1,4));
    REQUIRE(uses->storeProcUses(1,2));
    REQUIRE(uses->storeProcUses(1,3));
    REQUIRE(uses->storeProcUses(2,1));
    REQUIRE(uses->storeProcUses(2,3));
    REQUIRE(uses->storeProcUses(3,4));
    REQUIRE_FALSE(uses->storeProcUses(1,4));
    REQUIRE_FALSE(uses->storeProcUses(3,4));
}

TEST_CASE("getStmtSize Test [Uses]") {
    Uses* uses = setUpStmtUses();
    REQUIRE(uses->getStmtSize() == 6);
    uses->storeStmtUses(4, 5);
    REQUIRE(uses->getStmtSize() == 7);
}

TEST_CASE("getProcSize Test [Uses]") {
    Uses* uses = setUpProcUses();
    REQUIRE(uses->getProcSize() == 6);
    uses->storeProcUses(4, 5);
    REQUIRE(uses->getProcSize() == 7);
}

TEST_CASE("stmtUsesVar Test") {
    Uses* uses = setUpStmtUses();
    REQUIRE(uses->stmtUsesVar(1,1));
    REQUIRE(uses->stmtUsesVar(1,2));
    REQUIRE(uses->stmtUsesVar(1,3));
    REQUIRE(uses->stmtUsesVar(2,1));
    REQUIRE(uses->stmtUsesVar(2,3));
    REQUIRE(uses->stmtUsesVar(3,1));
    REQUIRE_FALSE(uses->stmtUsesVar(1,4));
}

TEST_CASE("procUsesVar Test") {
    Uses* uses = setUpProcUses();
    REQUIRE(uses->procUsesVar(1,4));
    REQUIRE(uses->procUsesVar(1,2));
    REQUIRE(uses->procUsesVar(1,3));
    REQUIRE(uses->procUsesVar(2,1));
    REQUIRE(uses->procUsesVar(2,3));
    REQUIRE(uses->procUsesVar(3,4));
    REQUIRE_FALSE(uses->procUsesVar(1,5));
}

TEST_CASE("getVarsUsedByStmt Test") {
    Uses* uses = setUpStmtUses();
    REQUIRE(uses->getVarsUsedByStmt(1) == unordered_set<VarID>({1,2,3}));
    REQUIRE(uses->getVarsUsedByStmt(2) == unordered_set<VarID>({1,3}));
    REQUIRE(uses->getVarsUsedByStmt(3) == unordered_set<VarID>({1}));
}

TEST_CASE("getVarsUsedByProc Test") {
    Uses* uses = setUpProcUses();
    REQUIRE(uses->getVarsUsedByProc(1) == unordered_set<VarID>({2,3,4}));
    REQUIRE(uses->getVarsUsedByProc(2) == unordered_set<VarID>({1,3}));
    REQUIRE(uses->getVarsUsedByProc(3) == unordered_set<VarID>({4}));
}

TEST_CASE("getStmtUses Test") {
    Uses* uses = setUpStmtUses();
    REQUIRE(uses->getStmtsUses(1) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(uses->getStmtsUses(2) == unordered_set<StmtNum>({1}));
    REQUIRE(uses->getStmtsUses(3) == unordered_set<StmtNum>({1,2}));
}

TEST_CASE("getProcUses Test") {
    Uses* uses = setUpProcUses();
    REQUIRE(uses->getProcsUses(1) == unordered_set<ProcID>({2}));
    REQUIRE(uses->getProcsUses(2) == unordered_set<ProcID>({1}));
    REQUIRE(uses->getProcsUses(3) == unordered_set<ProcID>({1,2}));
    REQUIRE(uses->getProcsUses(4) == unordered_set<ProcID>({1,3}));
}

TEST_CASE("getAllStmtUses Test") {
    Uses* uses = setUpStmtUses();
    pair<vector<StmtNum>, vector<VarID> > result = uses->getAllStmtUses();
    vector<StmtNum> stmts = result.first;
    vector<VarID> varIDs = result.second;
    int num_pairs = stmts.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == uses->getStmtSize());
    REQUIRE(num_pairs == varIDs.size());
    // check that each pair at the same index has Uses relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(uses->stmtUsesVar(stmts.at(i), varIDs.at(i)));
    }
}

TEST_CASE("getAllProcUses Test") {
    Uses* uses = setUpProcUses();
    pair<vector<ProcID>, vector<VarID> > result = uses->getAllProcUses();
    vector<ProcID> procIDs = result.first;
    vector<VarID> varIDs = result.second;
    int num_pairs = procIDs.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == uses->getProcSize());
    REQUIRE(num_pairs == varIDs.size());
    // check that each pair at the same index has Uses relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(uses->procUsesVar(procIDs.at(i), varIDs.at(i)));
    }
}
