#include "PQL/ParentEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index) : ast::Stmt(new sp::Token(), index) {};
};

void setupParent() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);

    PKB::parent = new Parent();
    PKB::parent->storeParent(1, 2);
    PKB::parent->storeParent(1, 3);
    PKB::parent->storeParent(5, 6);
    PKB::parent->storeParent(6, 7);
    PKB::parent->storeParent(8, 9);
    PKB::parent->storeParent(10, 11);
    PKB::parent->storeParent(10, 12);
    PKB::parent->storeParent(10, 13);
    PKB::parent->storeParent(10, 14);
}

TEST_CASE("ParentEvaluator evaluate known known") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"1", "2"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"10", "14"}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"5", "7"}, {}, 2), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults4;
    bool b4 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"11", "13"}, {}, 2), tempResults4);
    unordered_map<STRING, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("ParentEvaluator evaluate known synonym") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent", vector<STRING>{"5", "w"}, {"w"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"w", vector<int>{6}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Parent", vector<STRING>{"2", "a"}, {"a"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = ParentEvaluator::evaluate({ {"s", STMT_} }, Clause("Parent", vector<STRING>{"14", "s"}, {"s"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("ParentEvaluator evaluate known underscore") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"5", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"4", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentEvaluator evaluate synonym known") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({ {"ifs", IF_} }, Clause("Parent", vector<STRING>{"ifs", "2"}, {"ifs"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"ifs", vector<int>{1}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({ {"c", CALL_} }, Clause("Parent", vector<STRING>{"c", "5"}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = ParentEvaluator::evaluate({ {"s", STMT_} }, Clause("Parent", vector<STRING>{"s", "1"}, {"s"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("ParentEvaluator evaluate underscore known") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"_", "7"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"_", "8"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentEvaluator evaluate synonym synonym") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({ {"ifs", IF_}, {"c", CALL_} }, Clause("Parent", vector<STRING>{"ifs", "c"}, {"ifs", "c"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["ifs"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(8, 9)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({ {"ifs", IF_}, {"w", WHILE_} }, Clause("Parent", vector<STRING>{"ifs", "w"}, {"ifs", "w"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentEvaluator evaluate synonym underscore") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent", vector<STRING>{"w", "_"}, {"w"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
    unordered_set<int> expected1{ 5, 6 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Parent", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentEvaluator evaluate underscore synonym") {
    setupParent();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Parent", vector<STRING>{"_", "a"}, {"a"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1{ 2, 3, 7, 11, 12, 13, 14 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ParentEvaluator::evaluate({ {"p", PRINT_} }, Clause("Parent", vector<STRING>{"_", "p"}, {"p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentEvaluator evaluate underscore underscore") {
    setupParent();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ParentEvaluator::evaluate({}, Clause("Parent", vector<STRING>{"_", "_"}, {}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
