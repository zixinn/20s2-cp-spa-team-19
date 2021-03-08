#include "PQL/CallsTEvaluator.h"
#include "catch.hpp"
#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

void setUpCallsT() {
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

TEST_CASE("CallsTEvaluator evaluate underscore underscore (when proc Calls* exists)") {
    setUpCallsT();

    // Calls* (_, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"_", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("CallsTEvaluator evaluate underscore underscore (when proc Calls* does not exists)") {
    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls* (_, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"_", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("CallsTEvaluator evaluate known known") {
    setUpCallsT();

    // Calls* ("proc2", "proc3")
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({}, Clause("Calls*", vector<string>{"\"proc2\"", "\"proc3\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls* ("proc4", "proc0")
    unordered_map<string, vector<int>> tempResults3;
    bool b3 = CallsTEvaluator::evaluate({}, Clause("Calls*", vector<string>{"\"proc4\"", "\"proc0\""}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE(b3);
    REQUIRE(tempResults3 == expected3);

    // Calls* ("proc8", "proc4")
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({}, Clause("Calls*", vector<string>{"\"proc8\"", "\"proc4\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsTEvaluator evaluate known synonym") {
    setUpCallsT();

    // Calls* ("proc4", p)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"\"proc4\"", "p"}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 1, 2, 3, 5, 7, 0 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);
    
    // Calls* ("proc3", p)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"\"proc3\"", "p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsTEvaluator evaluate synonym known") {
    setUpCallsT();

    // Calls* (p, "proc7")
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"p", "\"proc7\""}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 4, 5, 6, 8 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);
    
    // Calls* (p, "proc4")
    unordered_map<string, vector<int>> tempResults3;
    bool b3 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"p", "\"proc4\""}), tempResults3);
    unordered_map<string, vector<int>> expected3 = { };
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("CallsTEvaluator evaluate known underscore") {
    setUpCallsT();

    // Calls* ("proc7", _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"\"proc7\"", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls* ("proc3", _)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"\"proc3\"", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsTEvaluator evaluate underscore known") {
    setUpCallsT();

    // Calls* (_, "proc1")
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"_", "\"proc1\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    // Calls* (_, "proc4")
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({ }, Clause("Calls*", vector<string>{"_", "\"proc4\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsTEvaluator evaluate synonym synonym") {
    setUpCallsT();

    // Calls* (p, p1)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_}, {"p1", PROCEDURE_} }, Clause("Calls*", vector<string>{"p", "p1"}), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["p1"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 1), make_pair(0, 2), make_pair(0, 3), 
        make_pair(1, 2), make_pair(1, 3), 
        make_pair(2, 3),
        make_pair(4, 1), make_pair(4, 2), make_pair(4, 3), make_pair(4, 5), make_pair(4, 7), make_pair(4, 0), 
        make_pair(5, 7), make_pair(5, 0), make_pair(5, 1), make_pair(5, 2), make_pair(5, 3), 
        make_pair(6, 7), make_pair(6, 0), make_pair(6, 1), make_pair(6, 2), make_pair(6, 3), 
        make_pair(8, 7), make_pair(8, 0), make_pair(8, 1), make_pair(8, 2), make_pair(8, 3), 
        make_pair(7, 0), make_pair(7, 1), make_pair(7, 2), make_pair(7, 3)} };

    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("CallsTEvaluator evaluate underscore synonym") {
    setUpCallsT();

    // Calls* (_, p)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"_", "p"}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 0, 1, 2, 3, 5, 7 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls* (_, p)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"_", "p"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("CallsTEvaluator evaluate synonym underscore") {
    setUpCallsT();

    // Calls* (p, _)
    unordered_map<string, vector<int>> tempResults1;
    bool b1 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"p", "_"}), tempResults1);
    unordered_set<int> actual1(tempResults1["p"].begin(), tempResults1["p"].end());
    unordered_set<int> expected1{ 0, 1, 2, 4, 5, 6, 7, 8 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    PKB::resetPKB();
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::calls->processCalls();
    // Calls* (p, _)
    unordered_map<string, vector<int>> tempResults2;
    bool b2 = CallsTEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Calls*", vector<string>{"p", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}
