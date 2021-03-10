#include "PQL/NextEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
};

void setupNext() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, READ_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, READ_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, PRINT_);

    PKB::next = new Next();
    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 2);
    PKB::next->storeNext(2, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(6, 8);
    PKB::next->storeNext(7, 9);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(9, 5);
    PKB::next->storeNext(5, 10);
    PKB::next->storeNext(10, 11);
    PKB::next->storeNext(10, 12);
    PKB::next->storeNext(11, 13);
    PKB::next->storeNext(12, 13);
    PKB::next->storeNext(13, 14);
}

void setupNext1() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);

    PKB::next = new Next();
    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 2);
}

TEST_CASE("NextEvaluator evaluate known known") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"3", "4"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"10", "11"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"7", "8"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<string, vector<int>> tempResults4;
    bool b4 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"2", "7"}), tempResults4);
    unordered_map<string, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("NextEvaluator evaluate known synonym") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({{"w", WHILE_}}, Clause("Next", vector<string>{"1", "w"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {{"w", vector<int>{2}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Next", vector<string>{"6", "a"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { {"a", vector<int>{7, 8}}};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = NextEvaluator::evaluate({{"s", STMT_}}, Clause("Next", vector<string>{"9", "s"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = { {"s", vector<int>{5}} };
    REQUIRE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<string, vector<int>> tempResults4;
    bool b4 = NextEvaluator::evaluate({ {"c", CALL_} }, Clause("Next", vector<string>{"2", "c"}), tempResults4);
    unordered_map<string, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("NextEvaluator evaluate known underscore") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"4", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"14", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextEvaluator evaluate synonym known") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({{"ifs", IF_}}, Clause("Next", vector<string>{"ifs", "12"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {{"ifs", vector<int>{10}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({{"r", READ_}}, Clause("Next", vector<string>{"r", "13"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { {"r", vector<int>{12}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = NextEvaluator::evaluate({{"p", PROGLINE_}}, Clause("Next", vector<string>{"p", "1"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("NextEvaluator evaluate underscore known") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"_", "8"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"_", "1"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextEvaluator evaluate synonym synonym") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({{"a", ASSIGN_}, {"c", CALL_}}, Clause("Next", vector<string>{"a", "c"}), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["a"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(3, 4), make_pair(7, 9), make_pair(8, 9)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({{"a", ASSIGN_}, {"ifs", IF_}}, Clause("Next", vector<string>{"a", "ifs"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextEvaluator evaluate synonym underscore") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({{"w", WHILE_}}, Clause("Next", vector<string>{"w", "_"}), tempResults1);
    unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
    unordered_set<int> expected1 {2, 5};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({{"pn", PRINT_}}, Clause("Next", vector<string>{"pn", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextEvaluator evaluate underscore synonym") {
    setupNext();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Next", vector<string>{"_", "a"}), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {3, 7, 8, 11};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    setupNext1();
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextEvaluator::evaluate({{"p", PRINT_}}, Clause("Next", vector<string>{"_", "p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextEvaluator evaluate underscore underscore") {
    setupNext();
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextEvaluator::evaluate({}, Clause("Next", vector<string>{"_", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
