#include "PQL/NextBipTEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
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

void setupNextBipT() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("Bill"); // 0
    PKB::procTable->storeProcName("Mary"); // 1
    PKB::procTable->storeProcName("John"); // 2
    PKB::procTable->storeProcStmt(0, 1, 5);
    PKB::procTable->storeProcStmt(1, 6, 8);
    PKB::procTable->storeProcStmt(2, 9, 9);

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

    PKB::next->populateNextStar();
    PKB::calls->processCalls();
    PKB::nextBip->setRunNextBip(true);
    PKB::nextBip->populateNextBipAndNextBipStar();
}

TEST_CASE("NextBipTEvaluator evaluate known known") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"10", "9"}, {}, 2), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"3", "6"}, {}, 2), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("NextBipTEvaluator evaluate known synonym") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({{"n", PROGLINE_}}, Clause("NextBip*", vector<string>{"5", "n"}, {"n"}, 1), tempResults1);
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({{"s", STMT_}}, Clause("NextBip*", vector<string>{"6", "s"}, {"s"}, 1), tempResults2);
    unordered_set<int> actual2(tempResults2["s"].begin(), tempResults2["s"].end());
    unordered_set<int> expected2 {3, 4, 5, 7, 8, 9, 10, 11};
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 1);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("NextBipTEvaluator evaluate known underscore") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"4", "_"}, {}, 1), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"14", "_"}, {}, 1), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("NextBipTEvaluator evaluate synonym known") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({{"r", READ_}}, Clause("NextBip*", vector<string>{"r", "4"}, {"r"}, 1), tempResults1);
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({{"ifs", IF_}}, Clause("NextBip*", vector<string>{"ifs", "5"}, {"ifs"}, 1), tempResults2);
    unordered_set<int> actual2(tempResults2["ifs"].begin(), tempResults2["ifs"].end());
    unordered_set<int> expected2 {9};
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 1);
    REQUIRE(actual2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = NextBipTEvaluator::evaluate({{"n", PROGLINE_}}, Clause("NextBip*", vector<string>{"n", "9"}, {"n"}, 1), tempResults3);
    unordered_set<int> actual3(tempResults3["n"].begin(), tempResults3["n"].end());
    unordered_set<int> expected3 {1, 2, 3, 4, 6, 7, 8, 9, 10, 11};
    REQUIRE(b3);
    REQUIRE(tempResults3.size() == 1);
    REQUIRE(actual3 == expected3);
}

TEST_CASE("NextBipTEvaluator evaluate underscore known") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"_", "6"}, {}, 1), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"_", "1"}, {}, 1), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("NextBipTEvaluator evaluate synonym synonym") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({{"c", CALL_}, {"s", STMT_}}, Clause("NextBip*", vector<string>{"c", "s"}, {"c", "s"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["s"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(2, 3), make_pair(2, 4), make_pair(2, 5), make_pair(2, 6), make_pair(2, 7),
                                      make_pair(2, 8), make_pair(2, 9), make_pair(2, 10), make_pair(2, 11), make_pair(4, 5),
                                      make_pair(4, 9), make_pair(4, 10), make_pair(4, 11), make_pair(7, 3), make_pair(7, 4),
                                      make_pair(7, 5), make_pair(7, 8), make_pair(7, 9), make_pair(7, 10), make_pair(7, 11)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({{"n", PROGLINE_}, {"w", WHILE_}}, Clause("NextBip*", vector<string>{"n", "w"}, {"n", "w"}, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("NextBipTEvaluator evaluate synonym underscore") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("NextBip*", vector<string>{"a", "_"}, {"a"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {1, 3, 6, 8, 10, 11};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({{"pn", PRINT_}}, Clause("NextBip*", vector<string>{"pn", "_"}, {"pn"}, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("NextBipTEvaluator evaluate underscore synonym") {
    setupNextBipT();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("NextBip*", vector<string>{"_", "a"}, {"a"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {3, 5, 6, 8, 10, 11};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = NextBipTEvaluator::evaluate({{"w", WHILE_}}, Clause("NextBip*", vector<string>{"_", "w"}, {"w"}, 0), tempResults2);
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2.empty());
}

TEST_CASE("NextBipTEvaluator evaluate underscore underscore") {
    setupNextBipT();
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = NextBipTEvaluator::evaluate({}, Clause("NextBip*", vector<string>{"_", "_"}, {}, 0), tempResults1);
    REQUIRE(b1);
    REQUIRE(tempResults1.empty());
}
