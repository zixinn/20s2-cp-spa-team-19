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
    stmtTable->storeIfPattern(1, 2);
    stmtTable->storeIfPattern(2, 1);
    stmtTable->storeWhilePattern(3, 1);
    stmtTable->storeWhilePattern(3, 2);
    stmtTable->storeIfPattern(6, 4);
    stmtTable->storeWhilePattern(9, 6);
    stmtTable->storeIfPattern(10, 3);
    stmtTable->storeIfPattern(13, 7);
    stmtTable->storeWhilePattern(16, 7);
    stmtTable->storeWhilePattern(17, 8);
    stmtTable->storeIfPattern(20, 10);
    stmtTable->storeIfPattern(22, 12);
    stmtTable->storeIfPattern(22, 13);
    stmtTable->storeWhilePattern(25, 14);
    stmtTable->storeWhilePattern(25, 15);
    return stmtTable;
}

StmtTable* setUpIfTests() {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeIfStmt(2, 3, set<int>{5});
    stmtTable->storeIfStmt(16, 17, set<int>{22});
    stmtTable->storeElseStmt(16, 23, set<int>{25});
    stmtTable->storeElseStmt(2, 6, set<int>{22, 25});
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

TEST_CASE("storeReadVariableForStmt and storePrintVariableForStmt Test") {
    StmtTable* stmtTable = new StmtTable();
    REQUIRE(stmtTable->storeReadVariableForStmt(1,2));
    REQUIRE_FALSE(stmtTable->storeReadVariableForStmt(1,3));
    REQUIRE(stmtTable->storeReadVariableForStmt(2,3));
    REQUIRE(stmtTable->storeReadVariableForStmt(3,2));
    REQUIRE(stmtTable->storePrintVariableForStmt(1,2));
    REQUIRE_FALSE(stmtTable->storePrintVariableForStmt(1,3));
    REQUIRE(stmtTable->storePrintVariableForStmt(2,3));
    REQUIRE(stmtTable->storePrintVariableForStmt(3,2));
}

TEST_CASE("storeIfPattern and storeWhilePattern Test") {
    StmtTable* stmtTable = new StmtTable();
    REQUIRE(stmtTable->storeIfPattern(1, 1));
    REQUIRE_FALSE(stmtTable->storeIfPattern(1,1));
    REQUIRE(stmtTable->storeIfPattern(1,2));
    REQUIRE(stmtTable->storeIfPattern(1,3));
    REQUIRE(stmtTable->storeIfPattern(2, 1));
    REQUIRE(stmtTable->storeIfPattern(1,5));
    REQUIRE(stmtTable->storeWhilePattern(3, 1));
    REQUIRE_FALSE(stmtTable->storeWhilePattern(3, 1));
    REQUIRE(stmtTable->storeWhilePattern(3,2));
    REQUIRE(stmtTable->storeWhilePattern(3, 3));
    REQUIRE_FALSE(stmtTable->storeWhilePattern(2,2));
    REQUIRE_FALSE(stmtTable->storeWhilePattern(3,2));
    REQUIRE(stmtTable->storeWhilePattern(4, 2));
    REQUIRE_FALSE(stmtTable->storeIfPattern(4,3));
}

TEST_CASE("storeIfStmt and storeElseStmt Test") {
    StmtTable* stmtTable = new StmtTable();
    REQUIRE(stmtTable->storeIfStmt(2, 3, set<int>{5}));
    REQUIRE_FALSE(stmtTable->storeIfStmt(2, 3, set<int>{5}));
    REQUIRE(stmtTable->storeIfStmt(16, 17, set<int>{22}));
    REQUIRE(stmtTable->storeElseStmt(16, 23, set<int>{25}));
    REQUIRE(stmtTable->storeElseStmt(2, 6, set<int>{22, 25}));
    REQUIRE_FALSE(stmtTable->storeElseStmt(2, 6, set<int>{22, 25}));
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
    REQUIRE(stmtTable->getIfPatternsSize() == 9);
}

TEST_CASE("getWhilePatternsSize Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getWhilePatternsSize() == 7);
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

TEST_CASE("getControlVarsOfIfStmt Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getControlVarsOfIfStmt(1) == unordered_set<ID>({1,2}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(2) == unordered_set<ID>({1}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(3).empty());
    REQUIRE(stmtTable->getControlVarsOfIfStmt(6) == unordered_set<ID>({4}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(9).empty());
    REQUIRE(stmtTable->getControlVarsOfIfStmt(10) == unordered_set<ID>({3}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(13) == unordered_set<ID>({7}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(16).empty());
    REQUIRE(stmtTable->getControlVarsOfIfStmt(17).empty());
    REQUIRE(stmtTable->getControlVarsOfIfStmt(20) == unordered_set<ID>({10}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(22) == unordered_set<ID>({12,13}));
    REQUIRE(stmtTable->getControlVarsOfIfStmt(25).empty());
    REQUIRE(stmtTable->getControlVarsOfIfStmt(30).empty());
}

TEST_CASE("getControlVarOfWhileStmt Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(1).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(2).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(3) == unordered_set<ID>({1,2}));
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(6).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(9) == unordered_set<ID>({6}));
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(10).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(13).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(16) == unordered_set<ID>({7}));
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(17) == unordered_set<ID>({8}));
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(20).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(22).empty());
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(25) == unordered_set<ID>({14,15}));
    REQUIRE(stmtTable->getControlVarsOfWhileStmt(30).empty());
}

TEST_CASE("getReadVariableOfStmt Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeReadVariableForStmt(1,2);
    stmtTable->storeReadVariableForStmt(2,3);
    stmtTable->storeReadVariableForStmt(3,2);
    stmtTable->storeReadVariableForStmt(5, 6);
    REQUIRE(stmtTable->getReadVariableOfStmt(1) == 2);
    REQUIRE(stmtTable->getReadVariableOfStmt(2) == 3);
    REQUIRE(stmtTable->getReadVariableOfStmt(3) == 2);
    REQUIRE(stmtTable->getReadVariableOfStmt(5) == 6);
    REQUIRE(stmtTable->getReadVariableOfStmt(4) == -1);
}

TEST_CASE("getPrintVariableOfStmt Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storePrintVariableForStmt(1,2);
    stmtTable->storePrintVariableForStmt(2,3);
    stmtTable->storePrintVariableForStmt(3,2);
    stmtTable->storePrintVariableForStmt(5, 6);
    REQUIRE(stmtTable->getPrintVariableOfStmt(1) == 2);
    REQUIRE(stmtTable->getPrintVariableOfStmt(2) == 3);
    REQUIRE(stmtTable->getPrintVariableOfStmt(3) == 2);
    REQUIRE(stmtTable->getPrintVariableOfStmt(5) == 6);
    REQUIRE(stmtTable->getPrintVariableOfStmt(4) == -1);
}

TEST_CASE("getStmtNumsOfReadWithVar Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storeReadVariableForStmt(1,2);
    stmtTable->storeReadVariableForStmt(2,3);
    stmtTable->storeReadVariableForStmt(3,2);
    stmtTable->storeReadVariableForStmt(5, 6);
    REQUIRE(stmtTable->getStmtNumsOfReadWithVar(2) == unordered_set<StmtNum>({1,3}));
    REQUIRE(stmtTable->getStmtNumsOfReadWithVar(3) == unordered_set<StmtNum>({2}));
    REQUIRE(stmtTable->getStmtNumsOfReadWithVar(6) == unordered_set<StmtNum>({5}));
    REQUIRE(stmtTable->getStmtNumsOfReadWithVar(1).empty());
}

TEST_CASE("getStmtNumsOfPrintWithVar Test") {
    StmtTable* stmtTable = new StmtTable();
    stmtTable->storePrintVariableForStmt(1,2);
    stmtTable->storePrintVariableForStmt(2,3);
    stmtTable->storePrintVariableForStmt(3,2);
    stmtTable->storePrintVariableForStmt(5,6);
    REQUIRE(stmtTable->getStmtNumsOfPrintWithVar(2) == unordered_set<StmtNum>({1,3}));
    REQUIRE(stmtTable->getStmtNumsOfPrintWithVar(3) == unordered_set<StmtNum>({2}));
    REQUIRE(stmtTable->getStmtNumsOfPrintWithVar(6) == unordered_set<StmtNum>({5}));
    REQUIRE(stmtTable->getStmtNumsOfPrintWithVar(1).empty());
}

TEST_CASE("isIfStmtWithControlVar Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE(stmtTable->isIfStmtWithControlVar(1,1));
    REQUIRE(stmtTable->isIfStmtWithControlVar(1,2));
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
    REQUIRE(stmtTable->isIfStmtWithControlVar(22,13));
    REQUIRE_FALSE(stmtTable->isIfStmtWithControlVar(25,14));
}

TEST_CASE("isWhileStmtWithControlVar Test") {
    StmtTable* stmtTable = setUpIfWhileTests();
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(1,1));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(2,1));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(3,2));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(3,1));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(6,4));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(9,6));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(10,3));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(13,7));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(16,7));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(17,8));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(20,10));
    REQUIRE_FALSE(stmtTable->isWhileStmtWithControlVar(22,12));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(25,14));
    REQUIRE(stmtTable->isWhileStmtWithControlVar(25,15));
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

TEST_CASE("getIfStmtRange and getElseStmtRange Test") {
    StmtTable* stmtTable = setUpIfTests();
    REQUIRE(stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{5}));
    REQUIRE(stmtTable->getElseStmtRange(2) == make_pair(6, set<int>{22, 25}));
    REQUIRE(stmtTable->getIfStmtRange(16) == make_pair(17, set<int>{22}));
    REQUIRE(stmtTable->getElseStmtRange(16) == make_pair(23, set<int>{25}));
}
