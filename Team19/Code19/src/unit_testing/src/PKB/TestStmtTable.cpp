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

StmtTable* setUpIfWhileTests() {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeIfPattern(1, 1);
    stmtTable->storeIfPattern(2, 1);
    stmtTable->storeWhilePattern(3, 2);
    stmtTable->storeIfPattern(6, 4);
    stmtTable->storeWhilePattern(9, 6);
    stmtTable->storeIfPattern(10, 3);
    stmtTable->storeIfPattern(13, 7);
    stmtTable->storeWhilePattern(16, 7);
    stmtTable->storeWhilePattern(17, 8);
    stmtTable->storeIfPattern(20, 10);
    stmtTable->storeIfPattern(22, 12);
    stmtTable->storeWhilePattern(25, 14);
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

TEST_CASE("storeIfPattern and storeWhilePattern Test") {
    StmtTable* stmtTable = new StmtTable();
    REQUIRE(stmtTable->storeIfPattern(1, 1));
    REQUIRE(stmtTable->storeIfPattern(2, 1));
    REQUIRE_FALSE(stmtTable->storeIfPattern(1,5));
    REQUIRE(stmtTable->storeWhilePattern(3, 1));
    REQUIRE_FALSE(stmtTable->storeWhilePattern(2,2));
    REQUIRE_FALSE(stmtTable->storeWhilePattern(3,2));
    REQUIRE(stmtTable->storeWhilePattern(4, 2));
    REQUIRE_FALSE(stmtTable->storeIfPattern(4,3));
}

TEST_CASE("getSize Test [StmtTable]") {
    StmtTable* stmtTable = setupStmtTestTable();
    REQUIRE(stmtTable->getSize() == 6);
    ast::Stmt* stmtNodeStub7 = new StmtNodeStub(7);
    stmtTable->storeStmt(7, stmtNodeStub7, "assign");
    REQUIRE(stmtTable->getSize() == 7);
}

TEST_CASE("getIfPatternsSize Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getIfPatternsSize() == 7);
}

TEST_CASE("getWhilePatternsSize Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getWhilePatternsSize() == 5);
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

TEST_CASE("getIfStmtsWithControlVar Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeIfPattern(1, 1);
    stmtTable->storeIfPattern(2, 1);
    stmtTable->storeIfPattern(3, 2);
    stmtTable->storeIfPattern(6, 4);
    stmtTable->storeIfPattern(9, 6);
    stmtTable->storeIfPattern(10, 3);
    REQUIRE(stmtTable->getIfStmtsWithControlVar(1) == unordered_set<StmtNum>({1,2}));
    REQUIRE(stmtTable->getIfStmtsWithControlVar(2) == unordered_set<StmtNum>({3}));
    REQUIRE(stmtTable->getIfStmtsWithControlVar(4) == unordered_set<StmtNum>({6}));
    REQUIRE(stmtTable->getIfStmtsWithControlVar(6) == unordered_set<StmtNum>({9}));
    REQUIRE(stmtTable->getIfStmtsWithControlVar(3) == unordered_set<StmtNum>({10}));
    REQUIRE(stmtTable->getIfStmtsWithControlVar(5).empty());
}

TEST_CASE("getWhileStmtsWithControlVar Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeWhilePattern(1, 1);
    stmtTable->storeWhilePattern(2, 1);
    stmtTable->storeWhilePattern(3, 2);
    stmtTable->storeWhilePattern(6, 4);
    stmtTable->storeWhilePattern(9, 6);
    stmtTable->storeWhilePattern(10, 3);
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(1) == unordered_set<StmtNum>({1,2}));
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(2) == unordered_set<StmtNum>({3}));
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(4) == unordered_set<StmtNum>({6}));
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(6) == unordered_set<StmtNum>({9}));
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(3) == unordered_set<StmtNum>({10}));
    REQUIRE(stmtTable->getWhileStmtsWithControlVar(5).empty());
}

TEST_CASE("getControlVarOfIfStmt Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getControlVarOfIfStmt(1) == 1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(2) == 1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(3) == -1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(6) == 4);
    REQUIRE(stmtTable->getControlVarOfIfStmt(9) == -1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(10) == 3);
    REQUIRE(stmtTable->getControlVarOfIfStmt(13) == 7);
    REQUIRE(stmtTable->getControlVarOfIfStmt(16) == -1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(17) == -1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(20) == 10);
    REQUIRE(stmtTable->getControlVarOfIfStmt(22) == 12);
    REQUIRE(stmtTable->getControlVarOfIfStmt(25) == -1);
    REQUIRE(stmtTable->getControlVarOfIfStmt(30) == -1);
}

TEST_CASE("getControlVarOfWhileStmt Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getControlVarOfWhileStmt(1) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(2) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(3) == 2);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(6) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(9) == 6);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(10) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(13) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(16) == 7);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(17) == 8);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(20) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(22) == -1);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(25) == 14);
    REQUIRE(stmtTable->getControlVarOfWhileStmt(30) == -1);
}

TEST_CASE("isIfStmtWithControlVar Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->isIfStmtWithControlVar(1,1));
    REQUIRE(stmtTable->isIfStmtWithControlVar(2,1));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(3,2));
    REQUIRE(stmtTable->isIfStmtWithControlVar(6,4));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(9,6));
    REQUIRE(stmtTable->isIfStmtWithControlVar(10,3));
    REQUIRE(stmtTable->isIfStmtWithControlVar(13,7));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(16,7));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(17,8));
    REQUIRE(stmtTable->isIfStmtWithControlVar(20,10));
    REQUIRE(stmtTable->isIfStmtWithControlVar(22,12));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(25,14));
}

TEST_CASE("isWhileStmtWithControlVar Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(1,1));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(2,1));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(3,2));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(6,4));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(9,6));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(10,3));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(13,7));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(16,7));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(17,8));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(20,10));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(22,12));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(25,14));
}

TEST_CASE("getAllIfPatterns Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    pair<vector<StmtNum>, vector<ID> > result = stmtTable->getAllIfPatterns();
    vector<StmtNum> stmtNums = result.first;
    vector<ID> controlVars = result.second;
    int num_pairs = stmtNums.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == stmtTable->getIfPatternsSize());
    REQUIRE(controlVars.size() == num_pairs);
    // check that each pair at the same index has Follows relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(stmtTable->isIfStmtWithControlVar(stmtNums.at(i), controlVars.at(i)));
    }
}

TEST_CASE("getAllWhilePatterns Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    pair<vector<StmtNum>, vector<ID> > result = stmtTable->getAllWhilePatterns();
    vector<StmtNum> stmtNums = result.first;
    vector<ID> controlVars = result.second;
    int num_pairs = stmtNums.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == stmtTable->getWhilePatternsSize());
    REQUIRE(controlVars.size() == num_pairs);
    // check that each pair at the same index has Follows relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(stmtTable->isWhileStmtWithControlVar(stmtNums.at(i), controlVars.at(i)));
    }
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
