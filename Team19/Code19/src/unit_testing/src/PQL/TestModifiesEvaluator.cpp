#include "PQL/ModifiesEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index): ast::Stmt(new sp::Token(), index){};
};

void setupModifies() {
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

    PKB::modifies = new Modifies();
    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(2, 1);
    PKB::modifies->storeStmtModifies(3, 2);
    PKB::modifies->storeStmtModifies(4, 3);
    PKB::modifies->storeStmtModifies(4, 4);
    PKB::modifies->storeStmtModifies(5, 0);
    PKB::modifies->storeStmtModifies(5, 1);
    PKB::modifies->storeStmtModifies(5, 2);
    PKB::modifies->storeStmtModifies(5, 3);
    PKB::modifies->storeStmtModifies(5, 4);
    PKB::modifies->storeStmtModifies(6, 0);
    PKB::modifies->storeStmtModifies(7, 1);
    PKB::modifies->storeStmtModifies(8, 2);
    PKB::modifies->storeStmtModifies(9, 3);
    PKB::modifies->storeStmtModifies(9, 4);
    PKB::modifies->storeStmtModifies(10, 5);
    PKB::modifies->storeStmtModifies(10, 1);
    PKB::modifies->storeStmtModifies(10, 2);
    PKB::modifies->storeStmtModifies(11, 5);
    PKB::modifies->storeStmtModifies(12, 1);
    PKB::modifies->storeStmtModifies(13, 2);
    PKB::modifies->storeStmtModifies(14, 6);

    PKB::modifies->storeProcModifies(0, 0);
    PKB::modifies->storeProcModifies(0, 1);
    PKB::modifies->storeProcModifies(0, 2);
    PKB::modifies->storeProcModifies(0, 3);
    PKB::modifies->storeProcModifies(0, 4);
    PKB::modifies->storeProcModifies(0, 5);
    PKB::modifies->storeProcModifies(0, 6);
}

TEST_CASE("ModifiesEvaluator evaluate stmt known known") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"7", "\"cenX\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"12", "\"flag\""}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate stmt known synonym") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"13", "v"}, {"v"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"v", vector<int>{2}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({{"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"4", "v"}, {"v"}, 1), tempResults2);
//    unordered_map<STRING, vector<int>> expected2 = {{"v", vector<int>{4, 3}}};
    unordered_set<int> actual2(tempResults2["v"].begin(), tempResults2["v"].end());
    unordered_set<int> expected2 {3, 4};
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate stmt known underscore") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"1", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"14", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate stmt synonym known") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"s", STMT_}}, Clause("Modifies", vector<STRING>{"s", "\"cenX\""}, {"s"}, 1), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"s", vector<int>{10, 5, 12, 7, 2}}};
    unordered_set<int> actual1(tempResults1["s"].begin(), tempResults1["s"].end());
    unordered_set<int> expected1 {2, 5, 7, 10, 12};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({{"w", WHILE_}}, Clause("Modifies", vector<STRING>{"w", "\"count\""}, {"w"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {{"w", vector<int>{5}}};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = ModifiesEvaluator::evaluate({{"ifs", IF_}}, Clause("Modifies", vector<STRING>{"ifs", "\"normSq\""}, {"ifs"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("ModifiesEvaluator evaluate stmt synonym synonym") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"ifs", IF_}, {"v", "variable"}}, Clause("Modifies", vector<STRING>{"ifs", "v"}, {"ifs", "v"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"ifs", vector<int>{10, 10, 10}}, {"v", vector<int>{2, 1, 5}}};
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["ifs"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(10, 1), make_pair(10, 2), make_pair(10, 5)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({{"r", READ_}, {"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"r", "v"}, {"r", "v"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate stmt synonym underscore") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Modifies", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{1, 2, 3, 6, 7, 8, 11, 12, 13, 14}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {1, 2, 3, 6, 7, 8, 11, 12, 13, 14};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({{"r", READ_}}, Clause("Modifies", vector<STRING>{"r", "_"}, {"r"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate proc known known") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"\"computeCentroid\"", "\"y\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({}, Clause("Modifies", vector<STRING>{"\"computeCentroid\"", "\"z\""}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate proc known synonym") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"\"computeCentroid\"", "v"}, {"v"}, 1), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"v", vector<int>{6, 5, 4, 3, 2, 1, 0}}};
    unordered_set<int> actual1(tempResults1["v"].begin(), tempResults1["v"].end());
    unordered_set<int> expected1 {0, 1, 2, 3, 4, 5, 6};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("ModifiesEvaluator evaluate proc known underscore") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"\"computeCentroid\"", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("ModifiesEvaluator evaluate proc synonym known") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"p", PROCEDURE_}}, Clause("Modifies", vector<STRING>{"p", "\"flag\""}, {"p"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"p", vector<int>{0}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = ModifiesEvaluator::evaluate({{"p", PROCEDURE_}}, Clause("Modifies", vector<STRING>{"p", "\"z\""}, {"p"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("ModifiesEvaluator evaluate proc synonym synonym") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"p", PROCEDURE_}, {"v", VARIABLE_}}, Clause("Modifies", vector<STRING>{"p", "v"}, {"p", "v"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"p", vector<int>{0, 0, 0, 0, 0, 0, 0}}, {"v", vector<int>{6, 5, 4, 3, 2, 1, 0}}};
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(0, 0), make_pair(0, 1), make_pair(0, 2), make_pair(0, 3), make_pair(0, 4), make_pair(0, 5), make_pair(0, 6)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("ModifiesEvaluator evaluate proc synonym underscore") {
    setupModifies();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = ModifiesEvaluator::evaluate({{"p", PROCEDURE_}}, Clause("Modifies", vector<STRING>{"p", "_"}, {"p"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"p", vector<int>{0}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
