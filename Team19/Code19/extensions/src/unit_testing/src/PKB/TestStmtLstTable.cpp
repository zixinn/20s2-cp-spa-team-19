#include "PKB/StmtLstTable.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("storeStmtLst Test") {
    StmtLstTable* stmtLstTable = new StmtLstTable();
    REQUIRE(stmtLstTable->storeStmtLst(1));
    REQUIRE(stmtLstTable->storeStmtLst(2));
    REQUIRE_FALSE(stmtLstTable->storeStmtLst(1));
}

TEST_CASE("getSize Test [StmtLstTable]") {
    StmtLstTable* stmtLstTable = new StmtLstTable();
    REQUIRE(stmtLstTable->getSize() == 0);
    stmtLstTable->storeStmtLst(1);
    stmtLstTable->storeStmtLst(2);
    REQUIRE(stmtLstTable->getSize() == 2);
}

TEST_CASE("hasStmtLst Test") {
    StmtLstTable* stmtLstTable = new StmtLstTable();
    stmtLstTable->storeStmtLst(1);
    stmtLstTable->storeStmtLst(2);
    REQUIRE(stmtLstTable->hasStmtLst(1));
    REQUIRE(stmtLstTable->hasStmtLst(2));
    REQUIRE_FALSE(stmtLstTable->hasStmtLst(3));
}

TEST_CASE("getAllStmtLsts Test") {
    StmtLstTable* stmtLstTable = new StmtLstTable();
    stmtLstTable->storeStmtLst(1);
    stmtLstTable->storeStmtLst(10);
    vector<StmtNum> const &firstStmtNums = stmtLstTable->getAllStmtLsts();
    REQUIRE_THAT(firstStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{10, 1}));
}
