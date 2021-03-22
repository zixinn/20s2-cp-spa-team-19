#include "PQL/CallsEvaluator.h"
#include "catch.hpp"
#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

void setUpCalls() {
    PKB::resetPKB();

    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::procTable->storeProcName("proc2"); // 2
    PKB::procTable->storeProcName("proc3"); // 3
    PKB::procTable->storeProcName("proc4"); // 4
    PKB::procTable->storeProcName("proc5"); // 5
    PKB::procTable->storeProcName("proc6"); // 6
    PKB::procTable->storeProcName("proc7"); // 7
    PKB::procTable->storeProcName("proc8"); // 8

    PKB::calls = new Calls();
    PKB::calls->storeCalls(1, 0, 1);
    PKB::calls->storeCalls(2, 1, 2);
    PKB::calls->storeCalls(3, 1, 3);
    PKB::calls->storeCalls(4, 2, 3);
    PKB::calls->storeCalls(5, 4, 1);
    PKB::calls->storeCalls(6, 4, 5);
    PKB::calls->storeCalls(7, 5, 7);
    PKB::calls->storeCalls(8, 6, 7);
    PKB::calls->storeCalls(9, 7, 0);
    PKB::calls->storeCalls(10, 8, 7);
    PKB::calls->processCalls();

    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, CALL_);
}

TEST_CASE("CallsEvaluator evaluate underscore underscore (when proc calls exists)") {
    setUpCalls();

    // Calls (_, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"_", "_"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("CallsEvaluator evaluate underscore underscore (when proc calls does not exists)") {
    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls (_, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"_", "_"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("CallsEvaluator evaluate known known") {
    setUpCalls();

    // Calls ("proc2", "proc3")
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({}, Clause("Calls", vector<string>{"\"proc2\"", "\"proc3\""}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls ("proc8", "proc0")
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({}, Clause("Calls", vector<string>{"\"proc8\"", "\"proc0\""}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsEvaluator evaluate known synonym") {
    setUpCalls();

    // Calls ("proc2", p)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"\"proc2\"", "p"}, {"p"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"p", vector<int>{3}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls ("proc3", p)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"\"proc3\"", "p"}, {"p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsEvaluator evaluate synonym known") {
    setUpCalls();

    // Calls (p, "proc7")
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"p", "\"proc7\""}, {"p"}), tempResults2);
    unordered_set<int> actual2(tempResults2["p"].begin(), tempResults2["p"].end());
    unordered_set<int> expected2{ 5, 6, 8 };
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);

    // Calls (p, "proc4")
    unordered_map<string, vector<int>> tempResults3;
    bool b3 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"p", "\"proc4\""}, {"p"}), tempResults3);
    unordered_map<string, vector<int>> expected3 = { };
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("CallsEvaluator evaluate known underscore") {
    setUpCalls();

    // Calls ("proc7", _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"\"proc7\"", "_"}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls ("proc3", _)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"\"proc3\"", "_"}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsEvaluator evaluate underscore known") {
    setUpCalls();

    // Calls (_, "proc1")
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"_", "\"proc1\""}, {}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls (_, "proc4")
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ }, Clause("Calls", vector<string>{"_", "\"proc4\""}, {}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsEvaluator evaluate synonym synonym") {
    setUpCalls();

    // Calls (p, p1)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ {"p", PROCEDURE_}, {"p1", PROCEDURE_} }, Clause("Calls", vector<string>{"p", "p1"}, {"p", "p1"}), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["p1"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 1), make_pair(1, 2), make_pair(1, 3), make_pair(2, 3), 
        make_pair(4, 1), make_pair(4, 5), make_pair(5, 7), make_pair(6, 7), make_pair(8, 7), make_pair(7, 0)} };

    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("CallsEvaluator evaluate underscore synonym") {
    setUpCalls();

    // Calls (_, p)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"_", "p"}, {"p"}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 0, 1, 2, 3, 5, 7 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls (_, p)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"_", "p"}, {"p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsEvaluator evaluate synonym underscore") {
    setUpCalls();

    // Calls (p, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"p", "_"}, {"p"}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 0, 1, 2, 4, 5, 6, 7, 8 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls (p, _)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls", vector<string>{"p", "_"}, {"p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}
