#include "PKB/Modifies.h"

#include "catch.hpp"
using namespace std;

Modifies* setUpStmtModifies() {
    Modifies* modifies = new Modifies();
    modifies->storeStmtModifies(1,1);
    modifies->storeStmtModifies(1,2);
    modifies->storeStmtModifies(1,3);
    modifies->storeStmtModifies(2,1);
    modifies->storeStmtModifies(2,3);
    modifies->storeStmtModifies(3,1);
    return modifies;
}

Modifies* setUpProcModifies() {
    Modifies* modifies = new Modifies();
    modifies->storeProcModifies(1,4);
    modifies->storeProcModifies(1,2);
    modifies->storeProcModifies(1,3);
    modifies->storeProcModifies(2,1);
    modifies->storeProcModifies(2,3);
    modifies->storeProcModifies(3,4);
    return modifies;
}

TEST_CASE("storeStmtModifies Test") {
    Modifies* modifies = new Modifies();
    REQUIRE(modifies->storeStmtModifies(1,1));
    REQUIRE(modifies->storeStmtModifies(1,2));
    REQUIRE(modifies->storeStmtModifies(1,3));
    REQUIRE(modifies->storeStmtModifies(2,1));
    REQUIRE(modifies->storeStmtModifies(2,3));
    REQUIRE(modifies->storeStmtModifies(3,1));
    REQUIRE_FALSE(modifies->storeStmtModifies(1,1));
    REQUIRE_FALSE(modifies->storeStmtModifies(3,1));
}

TEST_CASE("storeProcModifies Test") {
    Modifies* modifies = new Modifies();
    REQUIRE(modifies->storeProcModifies(1,4));
    REQUIRE(modifies->storeProcModifies(1,2));
    REQUIRE(modifies->storeProcModifies(1,3));
    REQUIRE(modifies->storeProcModifies(2,1));
    REQUIRE(modifies->storeProcModifies(2,3));
    REQUIRE(modifies->storeProcModifies(3,4));
    REQUIRE_FALSE(modifies->storeProcModifies(1,4));
    REQUIRE_FALSE(modifies->storeProcModifies(3,4));
}

TEST_CASE("getStmtSize Test [Modifies]") {
    Modifies* modifies = setUpStmtModifies();
    REQUIRE(modifies->getStmtSize() == 6);
    modifies->storeStmtModifies(4, 5);
    REQUIRE(modifies->getStmtSize() == 7);
}

TEST_CASE("getProcSize Test [Modifies]") {
    Modifies* modifies = setUpProcModifies();
    REQUIRE(modifies->getProcSize() == 6);
    modifies->storeProcModifies(4, 5);
    REQUIRE(modifies->getProcSize() == 7);
}

TEST_CASE("stmtModifiesVar Test") {
    Modifies* modifies = setUpStmtModifies();
    REQUIRE(modifies->stmtModifiesVar(1,1));
    REQUIRE(modifies->stmtModifiesVar(1,2));
    REQUIRE(modifies->stmtModifiesVar(1,3));
    REQUIRE(modifies->stmtModifiesVar(2,1));
    REQUIRE(modifies->stmtModifiesVar(2,3));
    REQUIRE(modifies->stmtModifiesVar(3,1));
    REQUIRE_FALSE(modifies->stmtModifiesVar(1,4));
}

TEST_CASE("procModifiesVar Test") {
    Modifies* modifies = setUpProcModifies();
    REQUIRE(modifies->procModifiesVar(1,4));
    REQUIRE(modifies->procModifiesVar(1,2));
    REQUIRE(modifies->procModifiesVar(1,3));
    REQUIRE(modifies->procModifiesVar(2,1));
    REQUIRE(modifies->procModifiesVar(2,3));
    REQUIRE(modifies->procModifiesVar(3,4));
    REQUIRE_FALSE(modifies->procModifiesVar(1,5));
}

TEST_CASE("getVarsModifiedByStmt Test") {
    Modifies* modifies = setUpStmtModifies();
    REQUIRE(modifies->getVarsModifiedByStmt(1) == unordered_set<VarID>({1,2,3}));
    REQUIRE(modifies->getVarsModifiedByStmt(2) == unordered_set<VarID>({1,3}));
    REQUIRE(modifies->getVarsModifiedByStmt(3) == unordered_set<VarID>({1}));
}

TEST_CASE("getVarsModifiedByProc Test") {
    Modifies* modifies = setUpProcModifies();
    REQUIRE(modifies->getVarsModifiedByProc(1) == unordered_set<VarID>({2,3,4}));
    REQUIRE(modifies->getVarsModifiedByProc(2) == unordered_set<VarID>({1,3}));
    REQUIRE(modifies->getVarsModifiedByProc(3) == unordered_set<VarID>({4}));
}

TEST_CASE("getStmtModifies Test") {
    Modifies* modifies = setUpStmtModifies();
    REQUIRE(modifies->getStmtsModifies(1) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(modifies->getStmtsModifies(2) == unordered_set<StmtNum>({1}));
    REQUIRE(modifies->getStmtsModifies(3) == unordered_set<StmtNum>({1,2}));
}

TEST_CASE("getProcModifies Test") {
    Modifies* modifies = setUpProcModifies();
    REQUIRE(modifies->getProcsModifies(1) == unordered_set<ProcID>({2}));
    REQUIRE(modifies->getProcsModifies(2) == unordered_set<ProcID>({1}));
    REQUIRE(modifies->getProcsModifies(3) == unordered_set<ProcID>({1,2}));
    REQUIRE(modifies->getProcsModifies(4) == unordered_set<ProcID>({1,3}));
}

TEST_CASE("getAllStmtModifies Test") {
    Modifies* modifies = setUpStmtModifies();
    pair<vector<StmtNum>, vector<VarID> > result = modifies->getAllStmtModifies();
    vector<StmtNum> stmts = result.first;
    vector<VarID> varIDs = result.second;
    int num_pairs = stmts.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == modifies->getStmtSize());
    REQUIRE(num_pairs == varIDs.size());
    // check that each pair at the same index has Modifies relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(modifies->stmtModifiesVar(stmts.at(i), varIDs.at(i)));
    }
}

TEST_CASE("getAllProcModifies Test") {
    Modifies* modifies = setUpProcModifies();
    pair<vector<ProcID>, vector<VarID> > result = modifies->getAllProcModifies();
    vector<ProcID> procIDs = result.first;
    vector<VarID> varIDs = result.second;
    int num_pairs = procIDs.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == modifies->getProcSize());
    REQUIRE(num_pairs == varIDs.size());
    // check that each pair at the same index has Modifies relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(modifies->procModifiesVar(procIDs.at(i), varIDs.at(i)));
    }
}
