#include "PQL/UsesEvaluator.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
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

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"7", "\"cenX\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"12", "\"flag\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt known synonym") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<string>{"13", "v"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"v", vector<int>{2}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<string>{"4", "v"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { {"v", vector<int>{4, 3}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt known underscore") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"1", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"14", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym known") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"s", STMT_} }, Clause("Uses", vector<string>{"s", "\"cenX\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"s", vector<int>{10, 5, 12, 7, 2}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"w", WHILE_} }, Clause("Uses", vector<string>{"w", "\"count\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = { {"w", vector<int>{5}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<string, vector<int>> tempResults3;
    bool b3 = UsesEvaluator::evaluate({ {"ifs", IF_} }, Clause("Uses", vector<string>{"ifs", "\"normSq\""}), tempResults3);
    unordered_map<string, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym synonym") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"ifs", IF_}, {"v", "variable"} }, Clause("Uses", vector<string>{"ifs", "v"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"ifs", vector<int>{10, 10, 10}}, {"v", vector<int>{2, 1, 5}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"r", READ_}, {"v", VARIABLE_} }, Clause("Uses", vector<string>{"r", "v"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate stmt synonym underscore") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"a", ASSIGN_} }, Clause("Uses", vector<string>{"a", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"a", vector<int>{1, 2, 3, 6, 7, 8, 11, 12, 13, 14}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"r", READ_} }, Clause("Uses", vector<string>{"r", "_"}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc known known") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"\"computeCentroid\"", "\"y\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({}, Clause("Uses", vector<string>{"\"computeCentroid\"", "\"z\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc known synonym") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<string>{"\"computeCentroid\"", "v"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"v", vector<int>{6, 5, 4, 3, 2, 1, 0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc known underscore") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("Uses", vector<string>{"\"computeCentroid\"", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc synonym known") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<string>{"p", "\"flag\""}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"p", vector<int>{0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<string, vector<int>> tempResults2;
    bool b2 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<string>{"p", "\"z\""}), tempResults2);
    unordered_map<string, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("UsesEvaluator evaluate proc synonym synonym") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("Uses", vector<string>{"p", "v"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"p", vector<int>{0, 0, 0, 0, 0, 0, 0}}, {"v", vector<int>{6, 5, 4, 3, 2, 1, 0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("UsesEvaluator evaluate proc synonym underscore") {
    setupUses();

    unordered_map<string, vector<int>> tempResults1;
    bool b1 = UsesEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("Uses", vector<string>{"p", "_"}), tempResults1);
    unordered_map<string, vector<int>> expected1 = { {"p", vector<int>{0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
