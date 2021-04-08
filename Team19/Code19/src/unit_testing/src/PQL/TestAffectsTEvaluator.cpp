#include "PQL/AffectsTEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index): ast::Stmt(new sp::Token(), index){};
};

//    procedure p {
//    1.  x = 1;
//    2.  y = 2;
//    3.  z = y;
//    4.  call q;
//    5.  z = x + y + z; }
//
//    procedure q {
//    6.  x = 5;
//    7.  t = 4;
//    8.  if (z > 0) then {
//    9.      t = x + 1;
//        } else {
//    10.     y = z + x; }
//    11. x = t + 1; }
//
//    proc ID: p = 1, q = 2
//    var ID: x = 1, y = 2, z = 3, t = 4
//
//    Affects(2,3)
//    Affects(3,5)
//    Affects(6,9), Affects(6,10)
//    Affects(7,11)
//    Affects(9,11)
//
//    Affects*(2,3), Affects*(2,5)
//    Affects*(3,5)
//    Affects*(6,9), Affects*(6,10), Affects*(6,11)
//    Affects*(7,11)
//    Affects*(9, 11)

void setupAffectsT() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(1);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);

    PKB::follows->storeFollows(1,2);
    PKB::follows->storeFollows(2,3);
    PKB::follows->storeFollows(3,4);
    PKB::follows->storeFollows(4,5);
    PKB::follows->storeFollows(6,7);
    PKB::follows->storeFollows(7,8);
    PKB::follows->storeFollows(8,11);

    PKB::parent->storeParent(8,9);
    PKB::parent->storeParent(8,10);

    PKB::uses->storeStmtUses(3,2);
    PKB::uses->storeStmtUses(5,1);
    PKB::uses->storeStmtUses(5,2);
    PKB::uses->storeStmtUses(5,3);
    PKB::uses->storeStmtUses(8,3);
    PKB::uses->storeStmtUses(8,1);
    PKB::uses->storeStmtUses(9,1);
    PKB::uses->storeStmtUses(10,1);
    PKB::uses->storeStmtUses(10,3);
    PKB::uses->storeStmtUses(11,4);

    PKB::uses->storeProcUses(1, 1);
    PKB::uses->storeProcUses(1, 2);
    PKB::uses->storeProcUses(1, 3);
    PKB::uses->storeProcUses(2, 1);
    PKB::uses->storeProcUses(2, 3);
    PKB::uses->storeProcUses(2, 4);

    PKB::modifies->storeStmtModifies(1,1);
    PKB::modifies->storeStmtModifies(2,2);
    PKB::modifies->storeStmtModifies(3,3);
    PKB::modifies->storeStmtModifies(5,3);
    PKB::modifies->storeStmtModifies(6,1);
    PKB::modifies->storeStmtModifies(7,4);
    PKB::modifies->storeStmtModifies(8,4);
    PKB::modifies->storeStmtModifies(8,2);
    PKB::modifies->storeStmtModifies(9,4);
    PKB::modifies->storeStmtModifies(10,2);
    PKB::modifies->storeStmtModifies(11,1);

    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(1,3);
    PKB::modifies->storeProcModifies(2,1);
    PKB::modifies->storeProcModifies(2,2);
    PKB::modifies->storeProcModifies(2,4);

    PKB::calls->storeCalls(4,1,2);

    PKB::next->storeNext(1,2);
    PKB::next->storeNext(2,3);
    PKB::next->storeNext(3,4);
    PKB::next->storeNext(4,5);
    PKB::next->storeNext(6,7);
    PKB::next->storeNext(7,8);
    PKB::next->storeNext(8,9);
    PKB::next->storeNext(8,10);
    PKB::next->storeNext(9,11);
    PKB::next->storeNext(10,11);

    PKB::nextBip->setRunNextBip(false);
    PKB::populatePKB();
}

//    procedure r {
//    1.  x = 1;
//    2.  y = z; }

void setupAffectsT2() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(1);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);

    PKB::follows->storeFollows(1,2);

    PKB::uses->storeStmtUses(2,3);
    PKB::uses->storeProcUses(1, 3);

    PKB::modifies->storeStmtModifies(1,1);
    PKB::modifies->storeStmtModifies(2,2);
    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);

    PKB::next->storeNext(1,2);

    PKB::nextBip->setRunNextBip(false);
    PKB::populatePKB();
}

//    procedure s {
//    1.  x = 9;
//    2.  while (x > 0) {
//    3.      y = y * 2;
//    4.      z = y;
//    5.      x = x - 1; }
//
//    proc ID: s = 1
//    var ID: x = 1, y = 2, z = 3
//
//    Affects(1,5)
//    Affects(3,3)
//    Affects(3,4)
//    Affects(5,5)
//
//    Affects*(1,5)
//    Affects*(3,3)
//    Affects*(3,4)
//    Affects*(5,5)

void setupAffectsT3() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(1);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);

    PKB::follows->storeFollows(1,2);
    PKB::follows->storeFollows(3,4);
    PKB::follows->storeFollows(4,5);

    PKB::parent->storeParent(2,3);
    PKB::parent->storeParent(2,4);
    PKB::parent->storeParent(2,5);

    PKB::uses->storeStmtUses(2,1);
    PKB::uses->storeStmtUses(2,2);
    PKB::uses->storeStmtUses(2,3);
    PKB::uses->storeStmtUses(3,2);
    PKB::uses->storeStmtUses(4,2);
    PKB::uses->storeStmtUses(5,1);

    PKB::uses->storeProcUses(1, 1);
    PKB::uses->storeProcUses(1, 2);
    PKB::uses->storeProcUses(1, 3);

    PKB::modifies->storeStmtModifies(1,1);
    PKB::modifies->storeStmtModifies(2,1);
    PKB::modifies->storeStmtModifies(2,2);
    PKB::modifies->storeStmtModifies(2,3);
    PKB::modifies->storeStmtModifies(3,2);
    PKB::modifies->storeStmtModifies(4,3);
    PKB::modifies->storeStmtModifies(5,1);

    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(1,3);

    PKB::next->storeNext(1,2);
    PKB::next->storeNext(2,3);
    PKB::next->storeNext(3,4);
    PKB::next->storeNext(4,5);
    PKB::next->storeNext(5,2);

    PKB::nextBip->setRunNextBip(false);
    PKB::populatePKB();
}

TEST_CASE("AffectsTEvaluator evaluate known known") {
    setupAffectsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"6", "11"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"8", "10"}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate known synonym") {
    setupAffectsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Affects*", vector<STRING>{"6", "a"}, {"a"}, 1), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{9, 10}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {9, 10, 11};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({{"n", PROGLINE_}}, Clause("Affects*", vector<STRING>{"3", "n"}, {"n"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {{"n", vector<int>{5}}};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsTEvaluator::evaluate({{"s", STMT_}}, Clause("Affects*", vector<STRING>{"4", "s"}, {"s"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("AffectsTEvaluator evaluate known underscore") {
    setupAffectsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"7", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"1", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate synonym known") {
    setupAffectsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Affects*", vector<STRING>{"a", "11"}, {"a"}, 1), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{7, 9}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {6, 7, 9};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({{"s", STMT_}}, Clause("Affects*", vector<STRING>{"s", "6"}, {"s"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate underscore known") {
    setupAffectsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"_", "9"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"_", "8"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate synonym synonym") {
    setupAffectsT();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({{"a", ASSIGN_}, {"n", PROGLINE_}}, Clause("Affects*", vector<STRING>{"a", "n"}, {"a", "n"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{2, 2, 3, 6, 6, 6, 7, 9}}, {"n", vector<int>{3, 5, 5, 9, 10, 11, 11, 11}}};
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["a"].at(i), tempResults1["n"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(2, 3), make_pair(2, 5), make_pair(3, 5), make_pair(6, 9),
                                             make_pair(6, 10), make_pair(6, 11), make_pair(7, 11), make_pair(9, 11)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    setupAffectsT2();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({{"a1", ASSIGN_}, {"a2", ASSIGN_}}, Clause("Affects*", vector<STRING>{"a1", "a2"}, {"a1", "a2"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    setupAffectsT3();
    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsTEvaluator::evaluate({ {"s", STMT_} }, Clause("Affects", vector<STRING>{"s", "s"}, {"s"}, 0), tempResults3);
    unordered_set<int> actual3(tempResults3["s"].begin(), tempResults3["s"].end());
    unordered_set<int> expected3 {3, 5};
    REQUIRE(b3);
    REQUIRE(actual3 == expected3);
}

TEST_CASE("AffectsTEvaluator evaluate synonym underscore") {
    setupAffectsT();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({{"s", STMT_}}, Clause("Affects*", vector<STRING>{"s", "_"}, {"s"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"s", vector<int>{2, 3, 6, 7, 9}}};
    unordered_set<int> actual1(tempResults1["s"].begin(), tempResults1["s"].end());
    unordered_set<int> expected1 {2, 3, 6, 7, 9};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    setupAffectsT2();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Affects*", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate underscore synonym") {
    setupAffectsT();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Affects*", vector<STRING>{"_", "a"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{3, 5, 9, 10, 11}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {3, 5, 9, 10, 11};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    setupAffectsT2();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({{"n", PROGLINE_}}, Clause("Affects*", vector<STRING>{"_", "n"}, {"n"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("AffectsTEvaluator evaluate underscore underscore") {
    setupAffectsT();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"_", "_"}, {}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    setupAffectsT2();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsTEvaluator::evaluate({}, Clause("Affects*", vector<STRING>{"_", "_"}, {}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}
