#include "PQL/UsesEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index) : ast::Stmt(new sp::Token(), index) {};
};

void setupUses() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);

    PKB::varTable = new VarTable();
    PKB::varTable->storeVarName("count"); // 0
    PKB::varTable->storeVarName("cenX"); // 1
    PKB::varTable->storeVarName("cenY"); // 2
    PKB::varTable->storeVarName("x"); // 3
    PKB::varTable->storeVarName("y"); // 4
    PKB::varTable->storeVarName("flag"); // 5
    PKB::varTable->storeVarName("normSq"); // 6
    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("computeCentroid"); // 0

    PKB::uses = new Uses();
    PKB::uses->storeStmtUses(1, 0);
    PKB::uses->storeStmtUses(2, 1);
    PKB::uses->storeStmtUses(3, 2);
    PKB::uses->storeStmtUses(4, 3);
    PKB::uses->storeStmtUses(4, 4);
    PKB::uses->storeStmtUses(5, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(5, 2);
    PKB::uses->storeStmtUses(5, 3);
    PKB::uses->storeStmtUses(5, 4);
    PKB::uses->storeStmtUses(6, 0);
    PKB::uses->storeStmtUses(7, 1);
    PKB::uses->storeStmtUses(8, 2);
    PKB::uses->storeStmtUses(9, 3);
    PKB::uses->storeStmtUses(9, 4);
    PKB::uses->storeStmtUses(10, 5);
    PKB::uses->storeStmtUses(10, 1);
    PKB::uses->storeStmtUses(10, 2);
    PKB::uses->storeStmtUses(11, 5);
    PKB::uses->storeStmtUses(12, 1);
    PKB::uses->storeStmtUses(13, 2);
    PKB::uses->storeStmtUses(14, 6);

    PKB::uses->storeProcUses(0, 0);
    PKB::uses->storeProcUses(0, 1);
    PKB::uses->storeProcUses(0, 2);
    PKB::uses->storeProcUses(0, 3);
    PKB::uses->storeProcUses(0, 4);
    PKB::uses->storeProcUses(0, 5);
    PKB::uses->storeProcUses(0, 6);
}

TEST_CASE("UsesEvaluator evaluate stmt known known") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"7", "\"cenX\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"12", "\"flag\""}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt known synonym") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"13", "v"}, {"v"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"v", vector<int>{2}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"4", "v"}, {"v"}, 1), tempResults2);
    unordered_set<int> actual2(tempResults2["v"].begin(), tempResults2["v"].end());
    unordered_set<int> expected2{ 3, 4 };
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt known underscore") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"1", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"14", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym known") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"s", STMT_} }, Clause("Uses", vector<STRING>{"s", "\"cenX\""}, {"s"}, 1), tempResults1);
    unordered_set<int> actual1(tempResults1["s"].begin(), tempResults1["s"].end());
    unordered_set<int> expected1{ 2, 5, 7, 10, 12 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"w", WHILE_} }, Clause("Uses", vector<STRING>{"w", "\"count\""}, {"w"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"w", vector<int>{5}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = UsesEvaluator::evaluate({ {"ifs", IF_} }, Clause("Uses", vector<STRING>{"ifs", "\"normSq\""}, {"ifs"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym synonym") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"ifs", IF_}, {"v", "variable"} }, Clause("Uses", vector<STRING>{"ifs", "v"}, {"ifs", "v"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["ifs"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(10, 1), make_pair(10, 2), make_pair(10, 5)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"r", READ_}, {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"r", "v"}, {"r", "v"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym underscore") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Uses", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1{ 1, 2, 3, 6, 7, 8, 11, 12, 13, 14 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"r", READ_} }, Clause("Uses", vector<STRING>{"r", "_"}, {"r"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc known known") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"\"computeCentroid\"", "\"y\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<STRING>{"\"computeCentroid\"", "\"z\""}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc known synonym") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"\"computeCentroid\"", "v"}, {"v"}, 1), tempResults1);
    unordered_set<int> actual1(tempResults1["v"].begin(), tempResults1["v"].end());
    unordered_set<int> expected1{ 0, 1, 2, 3, 4, 5, 6 };
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc known underscore") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"\"computeCentroid\"", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc synonym known") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<STRING>{"p", "\"flag\""}, {"p"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"p", vector<int>{0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<STRING>{"p", "\"z\""}, {"p"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc synonym synonym") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("Uses", vector<STRING>{"p", "v"}, {"p", "v"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 0), make_pair(0, 1), make_pair(0, 2), make_pair(0, 3), make_pair(0, 4), make_pair(0, 5), make_pair(0, 6)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc synonym underscore") {
    setupUses();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<STRING>{"p", "_"}, {"p"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"p", vector<int>{0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
