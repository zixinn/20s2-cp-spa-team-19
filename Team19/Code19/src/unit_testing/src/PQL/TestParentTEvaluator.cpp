#include "PQL/ParentTEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

void setupParentT() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, PRINT_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, READ_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, PRINT_);

    PKB::parent = new Parent();
    PKB::parent->storeParent(1, 2);
    PKB::parent->storeParent(1, 3);
    PKB::parent->storeParent(4, 5);
    PKB::parent->storeParent(5, 6);
    PKB::parent->storeParent(6, 7);
    PKB::parent->storeParent(10, 11);
    PKB::parent->storeParent(10, 12);
    PKB::parent->storeParent(10, 13);
    PKB::parent->storeParent(13, 14);
    PKB::parent->populateParentStar();
}

TEST_CASE("ParentTEvaluator evaluate known known") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"4", "5"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"4", "6"}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"9", "10"}, {}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<string, vector<int>> tempResults4;
    bool b4 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"2", "3"}, {}), tempResults4);
    unordered_map<string, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("ParentTEvaluator evaluate known synonym") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent*", vector<string>{"4", "w"}, {"w"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"w", vector<int>{6}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Parent*", vector<string>{"10", "a"}, {"a"}), tempResults2);
    unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
    unordered_set<int> expected2{ 11, 12 };
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = ParentTEvaluator::evaluate({ {"c", CALL_} }, Clause("Parent*", vector<string>{"13", "c"}, {"c"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("ParentTEvaluator evaluate known underscore") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"4", "_"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"11", "_"}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentTEvaluator evaluate synonym known") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent*", vector<string>{"w", "7"}, {"w"}), tempResults1);
    unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
    unordered_set<int> expected1{ 4, 6 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent*", vector<string>{"w", "14"}, {"w"}), tempResults2);
    unordered_set<int> actual2(tempResults2["w"].begin(), tempResults2["w"].end());
    unordered_set<int> expected2{ 13 };
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = ParentTEvaluator::evaluate({ {"ifs", IF_} }, Clause("Parent*", vector<string>{"ifs", "5"}, {"ifs"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("ParentTEvaluator evaluate underscore known") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"_", "6"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"_", "9"}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentTEvaluator evaluate synonym synonym") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({ {"ifs", IF_}, {"c", CALL_} }, Clause("Parent*", vector<string>{"ifs", "c"}, {"ifs", "c"}), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["ifs"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(1, 3)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({ {"ifs", IF_}, {"r", READ_} }, Clause("Parent*", vector<string>{"ifs", "r"}, {"ifs", "r"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentTEvaluator evaluate synonym underscore") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({ {"w", WHILE_} }, Clause("Parent*", vector<string>{"w", "_"}, {"w"}), tempResults1);
    unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
    unordered_set<int> expected1{ 4, 6, 13 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({ {"r", READ_} }, Clause("Parent*", vector<string>{"r", "_"}, {"r"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentTEvaluator evaluate underscore synonym") {
    setupParentT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Parent*", vector<string>{"_", "a"}, {"a"}), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1{ 2, 11, 12 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = ParentTEvaluator::evaluate({ {"r", READ_} }, Clause("Parent*", vector<string>{"_", "r"}, {"r"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ParentTEvaluator evaluate underscore underscore") {
    setupParentT();
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = ParentTEvaluator::evaluate({}, Clause("Parent*", vector<string>{"_", "_"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
