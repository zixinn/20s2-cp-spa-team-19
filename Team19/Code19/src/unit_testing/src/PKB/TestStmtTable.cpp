#include "PKB/StmtTable.h"

#include "catch.hpp"
using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
};

StmtTable* setupStmtTestTable() {
    StmtTable* stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub1 = new StmtNodeStub(1);
    ast::Stmt* stmtNodeStub2 = new StmtNodeStub(2);
    ast::Stmt* stmtNodeStub3 = new StmtNodeStub(3);
    ast::Stmt* stmtNodeStub4 = new StmtNodeStub(4);
    ast::Stmt* stmtNodeStub5 = new StmtNodeStub(5);
    ast::Stmt* stmtNodeStub6 = new StmtNodeStub(6);
    stmtTable->storeStmt(1, stmtNodeStub1, "assign");
    stmtTable->storeStmt(2, stmtNodeStub2, "read");
    stmtTable->storeStmt(3, stmtNodeStub3, "print");
    stmtTable->storeStmt(4, stmtNodeStub4, "call");
    stmtTable->storeStmt(5, stmtNodeStub5, "while");
    stmtTable->storeStmt(6, stmtNodeStub6, "if");
    return stmtTable;
}

TEST_CASE("storeStmt Test") {
    StmtTable* stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub1 = new StmtNodeStub(1);
    ast::Stmt* stmtNodeStub2 = new StmtNodeStub(2);
    REQUIRE(stmtTable->storeStmt(1, stmtNodeStub1, "assign"));
    // Check that stmtNum is also stored in AssignStmtNums
    vector<StmtNum> const &assignStmtNums = stmtTable->getAllAssignStmtNums();
    REQUIRE(find(assignStmtNums.begin(), assignStmtNums.end(), 1) != assignStmtNums.end());

    REQUIRE(stmtTable->storeStmt(2, stmtNodeStub2, "read"));
    // Check that stmtNum is also stored in WhileStmtNums
    vector<StmtNum> const &readStmtNums = stmtTable->getAllReadStmtNums();
    REQUIRE(find(readStmtNums.begin(), readStmtNums.end(), 2) != readStmtNums.end());

    // Check that all StmtNums has both stmt 1 and stmt2
    vector<StmtNum> const &stmtNums = stmtTable->getAllStmtNums();
    REQUIRE_THAT(stmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{1, 2}));

    // stmtNum 1 already exists
    REQUIRE_FALSE(stmtTable->storeStmt(1, stmtNodeStub1, "assign"));
}

TEST_CASE("storeAssignExpr Test") {
    StmtTable* stmtTable = new StmtTable();
    REQUIRE(stmtTable->storeAssignExpr(1, "v", "1+2"));
    REQUIRE(stmtTable->storeAssignExpr(2, "a", "a+b"));
    REQUIRE_FALSE(stmtTable->storeAssignExpr(1, "c", "a+b"));
}

TEST_CASE("getSize Test [StmtTable]") {
    StmtTable* stmtTable = setupStmtTestTable();
    REQUIRE(stmtTable->getSize() == 6);
    ast::Stmt* stmtNodeStub7 = new StmtNodeStub(7);
    stmtTable->storeStmt(7, stmtNodeStub7, "assign");
    REQUIRE(stmtTable->getSize() == 7);
}

TEST_CASE("hasStmt Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    REQUIRE(stmtTable->hasStmt(1));
    REQUIRE(stmtTable->hasStmt(2));
    REQUIRE_FALSE(stmtTable->hasStmt(9));
}

TEST_CASE("getAllAssignStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &assignStmtNums = stmtTable->getAllAssignStmtNums();
    REQUIRE_THAT(assignStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{1}));
}

TEST_CASE("getAllReadStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &readStmtNums = stmtTable->getAllReadStmtNums();
    REQUIRE_THAT(readStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{2}));
}

TEST_CASE("getAllPrintStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &printStmtNums = stmtTable->getAllPrintStmtNums();
    REQUIRE_THAT(printStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{3}));
}

TEST_CASE("getAllCallStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &callStmtNums = stmtTable->getAllCallStmtNums();
    REQUIRE_THAT(callStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{4}));
}

TEST_CASE("getAllWhileStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &whileStmtNums = stmtTable->getAllWhileStmtNums();
    REQUIRE_THAT(whileStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{5}));
}

TEST_CASE("getAllIfStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &ifStmtNums = stmtTable->getAllIfStmtNums();
    REQUIRE_THAT(ifStmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{6}));
}

TEST_CASE("getAllStmtNums Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    vector<StmtNum> const &stmtNums = stmtTable->getAllStmtNums();
    REQUIRE_THAT(stmtNums, Catch::Matchers::UnorderedEquals(vector<StmtNum>{1,3,5,4,2,6}));
}

TEST_CASE("getStmtNode Test") {
    StmtTable* stmtTable = setupStmtTestTable();
    REQUIRE(stmtTable->getStmtNode(1)->getIndex() == 1);
    REQUIRE(stmtTable->getStmtNode(2)->getIndex() == 2);
    REQUIRE_THROWS_AS(stmtTable->getStmtNode(-1), std::exception);
    REQUIRE_THROWS_AS(stmtTable->getStmtNode(7), std::exception);
}

TEST_CASE("getAssignExpr Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeAssignExpr(1, "v", "1+2");
    stmtTable->storeAssignExpr(2, "a", "a+b");
    pair<STRING, STRING> result1 = stmtTable->getAssignExpr(1);
    REQUIRE(result1.first == "v");
    REQUIRE(result1.second == "1+2");
    pair<STRING, STRING> result2 = stmtTable->getAssignExpr(2);
    REQUIRE(result2.first == "a");
    REQUIRE(result2.second == "a+b");
}
