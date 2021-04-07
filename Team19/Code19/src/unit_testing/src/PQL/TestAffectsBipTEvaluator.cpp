#include <set>

#include "PQL/AffectsBipTEvaluator.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index) : ast::Stmt(new sp::Token(), index) {};
};

//    procedure Bill {
//        01      x = 5;
//        02      call Mary;
//        03      y = x + 6;
//        04      call John;
//        05      z = x * y + 2; }
//
//    procedure Mary {
//        06      y = x * 3;
//        07      call John;
//        08      z = x + y; }
//
//    procedure John {
//        09      if (i > 0) then {
//        10              x = x + z; } }
//                else {
//        11              y = x * y; } }

void setupAffectsBipT1() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("Bill");  // 0
    PKB::procTable->storeProcName("Mary");  // 1
    PKB::procTable->storeProcName("John");  // 2
    PKB::procTable->storeProcStmt(0, 1, 5);
    PKB::procTable->storeProcStmt(1, 6, 8);
    PKB::procTable->storeProcStmt(2, 9, 9);

    PKB::varTable->storeVarName("x");  // 0
    PKB::varTable->storeVarName("y");  // 1
    PKB::varTable->storeVarName("z");  // 2
    PKB::varTable->storeVarName("i");  // 3

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(9, 10, set<StmtNum>{10});
    PKB::stmtTable->storeElseStmt(9, 11, set<StmtNum>{11});

    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(4, 0, 2);
    PKB::calls->storeCalls(7, 1, 2);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(7, 8);
    PKB::next->storeNext(9, 10);
    PKB::next->storeNext(9, 11);

    PKB::uses->storeStmtUses(2, 0);
    PKB::uses->storeStmtUses(2, 1);
    PKB::uses->storeStmtUses(2, 2);
    PKB::uses->storeStmtUses(2, 3);
    PKB::uses->storeStmtUses(3, 0);
    PKB::uses->storeStmtUses(4, 0);
    PKB::uses->storeStmtUses(4, 1);
    PKB::uses->storeStmtUses(4, 2);
    PKB::uses->storeStmtUses(5, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(6, 0);
    PKB::uses->storeStmtUses(7, 0);
    PKB::uses->storeStmtUses(7, 1);
    PKB::uses->storeStmtUses(7, 2);
    PKB::uses->storeStmtUses(8, 0);
    PKB::uses->storeStmtUses(8, 1);
    PKB::uses->storeStmtUses(9, 3);
    PKB::uses->storeStmtUses(10, 0);
    PKB::uses->storeStmtUses(10, 2);
    PKB::uses->storeStmtUses(11, 0);
    PKB::uses->storeStmtUses(11, 1);

    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(2, 0);
    PKB::modifies->storeStmtModifies(2, 1);
    PKB::modifies->storeStmtModifies(2, 2);
    PKB::modifies->storeStmtModifies(3, 1);
    PKB::modifies->storeStmtModifies(4, 0);
    PKB::modifies->storeStmtModifies(4, 1);
    PKB::modifies->storeStmtModifies(5, 2);
    PKB::modifies->storeStmtModifies(6, 1);
    PKB::modifies->storeStmtModifies(7, 0);
    PKB::modifies->storeStmtModifies(7, 1);
    PKB::modifies->storeStmtModifies(8, 2);
    PKB::modifies->storeStmtModifies(10, 0);
    PKB::modifies->storeStmtModifies(11, 1);

    PKB::calls->processCalls();
    PKB::nextBip->setRunNextBip(true);
    PKB::nextBip->populateNextBipAndNextBipStar();
    PKB::affectsBip->setRunAffectsBip(true);
    PKB::affectsBip->populateAffectsBipAndAffectsBipStar();
}

//    procedure B {
//      01      call C;
//      02      call C;
//      03      call C; }
//
//    procedure C {
//      04      d = a;
//      05      a = b;
//      06      b = c;
//      07      c = d; }

void setupAffectsBipT2() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("B");  // 0
    PKB::procTable->storeProcName("C");  // 1
    PKB::procTable->storeProcStmt(0, 1, 3);
    PKB::procTable->storeProcStmt(1, 4, 7);

    PKB::varTable->storeVarName("a");  // 0
    PKB::varTable->storeVarName("b");  // 1
    PKB::varTable->storeVarName("c");  // 2
    PKB::varTable->storeVarName("d");  // 3

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);

    PKB::calls->storeCalls(1, 0, 1);
    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(3, 0, 1);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);

    PKB::uses->storeStmtUses(1, 0);
    PKB::uses->storeStmtUses(1, 1);
    PKB::uses->storeStmtUses(1, 2);
    PKB::uses->storeStmtUses(1, 3);
    PKB::uses->storeStmtUses(2, 0);
    PKB::uses->storeStmtUses(2, 1);
    PKB::uses->storeStmtUses(2, 2);
    PKB::uses->storeStmtUses(2, 3);
    PKB::uses->storeStmtUses(3, 0);
    PKB::uses->storeStmtUses(3, 1);
    PKB::uses->storeStmtUses(3, 2);
    PKB::uses->storeStmtUses(3, 3);
    PKB::uses->storeStmtUses(4, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(6, 2);
    PKB::uses->storeStmtUses(7, 3);

    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(1, 1);
    PKB::modifies->storeStmtModifies(1, 2);
    PKB::modifies->storeStmtModifies(1, 3);
    PKB::modifies->storeStmtModifies(2, 0);
    PKB::modifies->storeStmtModifies(2, 1);
    PKB::modifies->storeStmtModifies(2, 2);
    PKB::modifies->storeStmtModifies(2, 3);
    PKB::modifies->storeStmtModifies(3, 0);
    PKB::modifies->storeStmtModifies(3, 1);
    PKB::modifies->storeStmtModifies(3, 2);
    PKB::modifies->storeStmtModifies(3, 3);
    PKB::modifies->storeStmtModifies(4, 3);
    PKB::modifies->storeStmtModifies(5, 0);
    PKB::modifies->storeStmtModifies(6, 1);
    PKB::modifies->storeStmtModifies(7, 2);

    PKB::calls->processCalls();
    PKB::nextBip->setRunNextBip(true);
    PKB::nextBip->populateNextBipAndNextBipStar();
    PKB::affectsBip->setRunAffectsBip(true);
    PKB::affectsBip->populateAffectsBipAndAffectsBipStar();
}

//    procedure A {
//      01      x = y;
//      02      call E;
//      03      y = z; }
//
//    procedure E {
//      04      b = y; }

void setupAffectsBipT3() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("A");  // 0
    PKB::procTable->storeProcName("E");  // 1
    PKB::procTable->storeProcStmt(0, 1, 3);
    PKB::procTable->storeProcStmt(1, 4, 4);

    PKB::varTable->storeVarName("x");  // 0
    PKB::varTable->storeVarName("y");  // 1
    PKB::varTable->storeVarName("z");  // 2
    PKB::varTable->storeVarName("b");  // 3

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);

    PKB::calls->storeCalls(2, 0, 1);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);

    PKB::uses->storeStmtUses(1, 1);
    PKB::uses->storeStmtUses(2, 1);
    PKB::uses->storeStmtUses(3, 2);
    PKB::uses->storeStmtUses(4, 1);

    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(2, 3);
    PKB::modifies->storeStmtModifies(3, 1);
    PKB::modifies->storeStmtModifies(4, 3);

    PKB::calls->processCalls();
    PKB::nextBip->setRunNextBip(true);
    PKB::nextBip->populateNextBipAndNextBipStar();
    PKB::affectsBip->setRunAffectsBip(true);
    PKB::affectsBip->populateAffectsBipAndAffectsBipStar();
}

TEST_CASE("AffectsBipTEvaluator evaluate known known") {
    setupAffectsBipT1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"6", "5"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"3", "8"}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    setupAffectsBipT2();
    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"5", "7"}, {}, 2), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults4;
    bool b4 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"7", "4"}, {}, 2), tempResults4);
    unordered_map<STRING, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("AffectsBipTEvaluator evaluate known synonym") {
    setupAffectsBipT1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("AffectsBip*", vector<STRING>{"5", "a"}, { "a" }, 1), tempResults1);
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1.empty());

    setupAffectsBipT2();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({ {"s", STMT_} }, Clause("AffectsBip*", vector<STRING>{"7", "s"}, { "s" }, 1), tempResults2);
    unordered_set<int> actual2(tempResults2["s"].begin(), tempResults2["s"].end());
    unordered_set<int> expected2{ 6, 5 };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 1);
    REQUIRE(actual2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsBipTEvaluator::evaluate({ {"n", PROGLINE_} }, Clause("AffectsBip*", vector<STRING>{"4", "n"}, { "n" }, 1), tempResults3);
    unordered_set<int> actual3(tempResults3["n"].begin(), tempResults3["n"].end());
    unordered_set<int> expected3{ 7, 6, 5 };
    REQUIRE(b3);
    REQUIRE(tempResults3.size() == 1);
    REQUIRE(actual3 == expected3);
}

TEST_CASE("AffectsBipTEvaluator evaluate known underscore") {
    setupAffectsBipT1();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"6", "_"}, {}, 1), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"5", "_"}, {}, 1), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("AffectsBipTEvaluator evaluate synonym known") {
    setupAffectsBipT1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({ {"s", STMT_} }, Clause("AffectsBip*", vector<STRING>{"s", "1"}, { "s" }, 1), tempResults1);
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("AffectsBip*", vector<STRING>{"a", "5"}, { "a" }, 1), tempResults2);
    unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
    unordered_set<int> expected2{ 1, 3, 8, 6, 10, 11 };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 1);
    REQUIRE(actual2 == expected2);

    setupAffectsBipT2();
    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsBipTEvaluator::evaluate({ {"n", PROGLINE_} }, Clause("AffectsBip*", vector<STRING>{"n", "7"}, { "n" }, 1), tempResults3);
    unordered_set<int> actual3(tempResults3["n"].begin(), tempResults3["n"].end());
    unordered_set<int> expected3{ 4, 5, 6 };
    REQUIRE(b3);
    REQUIRE(tempResults3.size() == 1);
    REQUIRE(actual3 == expected3);
}

TEST_CASE("AffectsBipTEvaluator evaluate underscore known") {
    setupAffectsBipT1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"_", "5"}, {}, 1), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"_", "1"}, {}, 1), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("AffectsBipTEvaluator evaluate synonym synonym") {
    setupAffectsBipT2();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({ {"a", ASSIGN_}, {"s", STMT_} }, 
                                             Clause("AffectsBip*", vector<STRING>{"a", "s"}, { "a", "s" }, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p =
            make_pair(tempResults1["a"].at(i), tempResults1["s"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 5), make_pair(5, 4),
                                      make_pair(6, 5), make_pair(7, 6),
                                      make_pair(4, 7), make_pair(6, 7),
                                      make_pair(5, 7), make_pair(6, 4),
                                      make_pair(7, 5), make_pair(4, 6),
                                      make_pair(5, 6)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    setupAffectsBipT3();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({ {"n", PROGLINE_}, {"n", PROGLINE_} },
                                             Clause("AffectsBip*", vector<STRING>{"n", "n"}, { "n", "n" }, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("AffectsBipTEvaluator evaluate synonym underscore") {
    setupAffectsBipT1();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("AffectsBip*", vector<STRING>{"a", "_"}, { "a" }, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1{ 1, 3, 6, 8, 10, 11 };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);

    setupAffectsBipT3();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({ {"n", PROGLINE_} }, Clause("AffectsBip*", vector<STRING>{"n", "_"}, { "n" }, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("AffectsBipTEvaluator evaluate underscore synonym") {
    setupAffectsBipT1();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("AffectsBip*", vector<STRING>{"_", "a"}, { "a" }, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1{ 3, 5, 6, 8, 10, 11 };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);

    setupAffectsBipT3();
    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = AffectsBipTEvaluator::evaluate({ {"s", STMT_} }, Clause("AffectsBip*", vector<STRING>{"_", "s"}, { "s" }, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("AffectsBipTEvaluator evaluate underscore underscore") {
    setupAffectsBipT1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"_", "_"}, {}, 0), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());

    setupAffectsBipT3();
    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = AffectsBipTEvaluator::evaluate({}, Clause("AffectsBip*", vector<STRING>{"_", "_"}, {}, 0), tempResults3);
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3.empty());
}
