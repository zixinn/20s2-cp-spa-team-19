#include "PQL/WithEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index) : ast::Stmt(new sp::Token(), index) {};
};

// test constants first
void setupWith() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_); //0 calls 1
    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, READ_); // read x
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_); // 1 calls 2
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, PRINT_); // print v
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, READ_); // read x
    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeReadVariableForStmt(8, 0);
    PKB::stmtTable->storeReadVariableForStmt(13, 0);
    PKB::stmtTable->storePrintVariableForStmt(11, 1);

    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("2");
    PKB::constTable->storeConst("290");

    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1
    PKB::procTable->storeProcName("proc2"); // 2

    PKB::calls = new Calls();
    PKB::calls->storeCalls(4, 0, 1);
    PKB::calls->storeCalls(9, 1, 2);
    PKB::calls->processCalls();

    PKB::varTable = new VarTable();
    PKB::varTable->storeVarName("x"); // 0
    PKB::varTable->storeVarName("v"); // 1
    PKB::varTable->storeVarName("cenX"); // 2
    PKB::varTable->storeVarName("urgh"); // 3
}

void setupWith1() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_); //calls proc1
    PKB::stmtTable->storeStmt(3, stmtNodeStub, PRINT_); // print proc0
    PKB::stmtTable->storeStmt(8, stmtNodeStub, READ_); // read proc0
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_); // calls proc0
    PKB::stmtTable->storeStmt(11, stmtNodeStub, PRINT_); // print proc0
    PKB::stmtTable->storeStmt(13, stmtNodeStub, READ_); // read proc1
    PKB::stmtTable->storeReadVariableForStmt(8, 0);
    PKB::stmtTable->storeReadVariableForStmt(13, 1);
    PKB::stmtTable->storePrintVariableForStmt(3, 1);
    PKB::stmtTable->storePrintVariableForStmt(11, 1);

    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("proc0"); // 0
    PKB::procTable->storeProcName("proc1"); // 1

    PKB::varTable = new VarTable();
    PKB::varTable->storeVarName("proc1"); // 0
    PKB::varTable->storeVarName("proc0"); // 1

    PKB::calls = new Calls();
    PKB::calls->storeCalls(4, 0, 1);
    PKB::calls->storeCalls(9, 1, 0);
    PKB::calls->processCalls();
}

TEST_CASE("With evaluator int = int (positive case)") {
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ }, Clause("", vector<STRING>{"2", "2"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("With evaluator int = int (negative case)") {
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ }, Clause("", vector<STRING>{"2", "3"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("With evaluator name = name (positive case)") {
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ }, Clause("", vector<STRING>{"\"blah\"", "\"blah\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("With evaluator name = name (negative case)") {
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ }, Clause("", vector<STRING>{"\"blah1\"", "\"blah\""}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("With Evaluator p1.procName = p2.procName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p1", PROCEDURE_}, {"p2", PROCEDURE_} }, Clause("", vector<STRING>{"p1.procName", "p2.procName"}, {"p1", "p2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p1"].at(i), tempResults1["p2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 0), make_pair(1, 1), make_pair(2, 2)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator c1.procName = c2.procName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c1", CALL_}, {"c2", CALL_} }, Clause("", vector<STRING>{"c1.procName", "c2.procName"}, {"c1", "c2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c1"].at(i), tempResults1["c2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 4), make_pair(9, 9)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator v1.varName = v2.varName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v1", VARIABLE_}, {"v2", VARIABLE_} }, Clause("", vector<STRING>{"v1.varName", "v2.varName"}, {"v1", "v2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["v1"].at(i), tempResults1["v2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 0), make_pair(1, 1), make_pair(2, 2), make_pair(3, 3)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator r1.varName = r2.varName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"r1", READ_}, {"r2", READ_} }, Clause("", vector<STRING>{"r1.varName", "r2.varName"}, {"r1", "r2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["r1"].at(i), tempResults1["r2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(8, 8), make_pair(13, 13), make_pair(8, 13), make_pair(13, 8)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator p1.varName = p2.varName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p1", PRINT_}, {"p2", PRINT_} }, Clause("", vector<STRING>{"p1.varName", "p2.varName"}, {"p1", "p2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p1"].at(i), tempResults1["p2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(11, 11)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator const1.value = const2.value") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"const1", CONSTANT_}, {"const2", CONSTANT_} }, Clause("", vector<STRING>{"const1.value", "const2.value"}, {"const1", "const2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["const1"].at(i), tempResults1["const2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(2, 2), make_pair(290, 290)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator s1.stmt# = s2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"s1", STMT_}, {"s2", STMT_} }, Clause("", vector<STRING>{"s1.stmt#", "s2.stmt#"}, {"s1", "s2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["s1"].at(i), tempResults1["s2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(1, 1), make_pair(2, 2), make_pair(3, 3), make_pair(4, 4), make_pair(5, 5),
                                              make_pair(6, 6), make_pair(7, 7), make_pair(8, 8), make_pair(9, 9), make_pair(10, 10), make_pair(11, 11), make_pair(12, 12),
                                              make_pair(13, 13), make_pair(14, 14)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator r1.stmt# = r2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"r1", READ_}, {"r2", READ_} }, Clause("", vector<STRING>{"r1.stmt#", "r2.stmt#"}, {"r1", "r2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["r1"].at(i), tempResults1["r2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(8, 8), make_pair(13, 13)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator p1.stmt# = p2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p1", PRINT_}, {"p2", PRINT_} }, Clause("", vector<STRING>{"p1.stmt#", "p2.stmt#"}, {"p1", "p2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p1"].at(i), tempResults1["p2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(11, 11)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator c1.stmt# = c2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c1", CALL_}, {"c2", CALL_} }, Clause("", vector<STRING>{"c1.stmt#", "c2.stmt#"}, {"c1", "c2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c1"].at(i), tempResults1["c2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 4), make_pair(9, 9)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator w1.stmt# = w2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"w1", WHILE_}, {"w2", WHILE_} }, Clause("", vector<STRING>{"w1.stmt#", "w2.stmt#"}, {"w1", "w2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["w1"].at(i), tempResults1["w2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(5, 5)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator ifs1.stmt# = ifs2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"ifs1", IF_}, {"ifs2", IF_} }, Clause("", vector<STRING>{"ifs1.stmt#", "ifs2.stmt#"}, {"ifs1", "ifs2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["ifs1"].at(i), tempResults1["ifs2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(10, 10)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With Evaluator a1.stmt# = a2.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"a1", ASSIGN_}, {"a2", ASSIGN_} }, Clause("", vector<STRING>{"a1.stmt#", "a2.stmt#"}, {"a1", "a2"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["a1"].at(i), tempResults1["a2"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(1, 1), make_pair(2, 2), make_pair(3, 3), make_pair(6, 6), make_pair(7, 7), make_pair(12, 12), make_pair(14, 14)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With evaluator name = p.procName / p.procName = name (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("", vector<STRING>{"\"proc0\"", "p.procName"}, {"p"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"p", vector<int>{0}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("", vector<STRING>{"p.procName", "\"proc1\""}, {"p"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"p", vector<int>{1}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = p.procName / p.procName = name (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("", vector<STRING>{"\"proc00\"", "p.procName"}, {"p"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_} }, Clause("", vector<STRING>{"p.procName", "\"proc5\""}, {"p"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = c.procName / c.procName = name (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_} }, Clause("", vector<STRING>{"\"proc2\"", "c.procName"}, {"c"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"c", vector<int>{9}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_} }, Clause("", vector<STRING>{"c.procName", "\"proc1\""}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"c", vector<int>{4}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = c.procName / c.procName = name (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_} }, Clause("", vector<STRING>{"\"proc0\"", "c.procName"}, {"c"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_} }, Clause("", vector<STRING>{"c.procName", "\"proc6\""}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = v.varName / v.varName = name (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("", vector<STRING>{"\"urgh\"", "v.varName"}, {"v"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"v", vector<int>{3}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "\"v\""}, {"v"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"v", vector<int>{1}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = v.varName / v.varName = name (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("", vector<STRING>{"\"urgh2\"", "v.varName"}, {"v"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "\"urgh2\""}, {"v"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = r.varName / r.varName = name (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"r", READ_} }, Clause("", vector<STRING>{"\"x\"", "r.varName"}, {"r"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"r", vector<int>{8, 13}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "\"x\""}, {"r"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"r", vector<int>{8, 13}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = r.varName / r.varName = name (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"r", READ_} }, Clause("", vector<STRING>{"\"p00\"", "r.varName"}, {"r"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "\"cenX\""}, {"r"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = pn.varName / pn.varName = name (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_} }, Clause("", vector<STRING>{"\"v\"", "pn.varName"}, {"pn"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"pn", vector<int>{11}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "\"v\""}, {"pn"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { {"pn", vector<int>{11}} };
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator name = pn.varName / pn.varName = name (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_} }, Clause("", vector<STRING>{"\"x\"", "pn.varName"}, {"pn"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "\"x\""}, {"pn"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator p.procName = c.procName / c.procName = p.procName (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"c", CALL_} }, Clause("", vector<STRING>{"p.procName", "c.procName"}, {"p", "c"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(1, 4), make_pair(2, 9)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"c", CALL_} }, Clause("", vector<STRING>{"c.procName", "p.procName"}, {"c", "p"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["c"].at(i), tempResults2["p"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(4, 1), make_pair(9, 2)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator p.procName = c.procName / c.procName = p.procName (negative case)") {
    PKB::procTable = new ProcTable();
    PKB::calls = new Calls();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"c", CALL_} }, Clause("", vector<STRING>{"p.procName", "c.procName"}, {"p", "c"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"c", CALL_} }, Clause("", vector<STRING>{"c.procName", "p.procName"}, {"c", "p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator p.procName = v.varName / v.varName = p.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"p.procName", "v.varName"}, {"p", "v"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 1), make_pair(1, 0)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "p.procName"}, {"v", "p"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["v"].at(i), tempResults2["p"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(1, 0), make_pair(0, 1)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator p.procName = v.varName / v.varName = p.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"p.procName", "v.varName"}, {"p", "v"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "p.procName"}, {"v", "p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator p.procName = r.varName / r.varName = p.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"r", READ_} }, Clause("", vector<STRING>{"p.procName", "r.varName"}, {"p", "r"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["r"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 13), make_pair(1, 8)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "p.procName"}, {"r", "p"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["r"].at(i), tempResults2["p"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(13, 0), make_pair(8, 1)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator p.procName = r.varName / r.varName = p.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"r", READ_} }, Clause("", vector<STRING>{"p.procName", "r.varName"}, {"p", "r"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "p.procName"}, {"r", "p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator p.procName = pn.varName / pn.varName = p.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"p.procName", "pn.varName"}, {"p", "pn"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["p"].at(i), tempResults1["pn"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 3), make_pair(0, 11)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "p.procName"}, {"pn", "p"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["pn"].at(i), tempResults2["p"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(3, 0), make_pair(11, 0)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator p.procName = pn.varName / pn.varName = p.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"p.procName", "pn.varName"}, {"p", "pn"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"p", PROCEDURE_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "p.procName"}, {"pn", "p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator c.procName = v.varName / v.varName = c.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"c.procName", "v.varName"}, {"c", "v"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 0), make_pair(9, 1)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "c.procName"}, {"v", "c"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["v"].at(i), tempResults2["c"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(0, 4), make_pair(1, 9)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator c.procName = v.varName / v.varName = c.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"c.procName", "v.varName"}, {"c", "v"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "c.procName"}, {"v", "c"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator c.procName = r.varName / r.varName = c.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"r", READ_} }, Clause("", vector<STRING>{"c.procName", "r.varName"}, {"c", "r"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["r"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 8), make_pair(9, 13)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "c.procName"}, {"r", "c"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["r"].at(i), tempResults2["c"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(8, 4), make_pair(13, 9)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator c.procName = r.varName / r.varName = c.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"r", READ_} }, Clause("", vector<STRING>{"c.procName", "r.varName"}, {"c", "r"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "c.procName"}, {"r", "c"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator c.procName = pn.varName / pn.varName = c.procName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"c.procName", "pn.varName"}, {"c", "pn"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["pn"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(9, 3), make_pair(9, 11)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "c.procName"}, {"pn", "c"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["pn"].at(i), tempResults2["c"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(3, 9), make_pair(11, 9)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator c.procName = pn.varName / pn.varName = c.procName (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"c.procName", "pn.varName"}, {"c", "pn"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CALL_}, {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.varName", "c.procName"}, {"pn", "c"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator v.varName = r.varName / r.varName = v.varName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v", VARIABLE_}, {"r", READ_} }, Clause("", vector<STRING>{"v.varName", "r.varName"}, {"v", "r"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["v"].at(i), tempResults1["r"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(0, 8), make_pair(1, 13)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"v", VARIABLE_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "v.varName"}, {"r", "v"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["r"].at(i), tempResults2["v"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(8, 0), make_pair(13, 1)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator v.varName = r.varName / r.varName = v.varName (negative case)") {
    PKB::stmtTable = new StmtTable();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v", VARIABLE_}, {"r", READ_} }, Clause("", vector<STRING>{"v.varName", "r.varName"}, {"v", "r"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"v", VARIABLE_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "v.varName"}, {"r", "v"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator v.varName = pn.varName / pn.varName = v.varName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"pn.varName", "v.varName"}, {"pn", "v"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["pn"].at(i), tempResults1["v"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(3, 1), make_pair(11, 1)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "pn.varName"}, {"v", "pn"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["v"].at(i), tempResults2["pn"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(1, 3), make_pair(1, 11)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator v.varName = pn.varName / pn.varName = v.varName (negative case)") {
    PKB::stmtTable = new StmtTable();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"pn.varName", "v.varName"}, {"pn", "v"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"v", VARIABLE_} }, Clause("", vector<STRING>{"v.varName", "pn.varName"}, {"v", "pn"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator r.varName = pn.varName / pn.varName = r.varName (positive case)") {
    setupWith1();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"r", READ_} }, Clause("", vector<STRING>{"pn.varName", "r.varName"}, {"pn", "r"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["pn"].at(i), tempResults1["r"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(3, 13), make_pair(11, 13)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "pn.varName"}, {"r", "pn"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["r"].at(i), tempResults2["pn"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(13, 3), make_pair(13, 11)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator r.varName = pn.varName / pn.varName = r.varName (negative case)") {
    PKB::stmtTable = new StmtTable();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"r", READ_} }, Clause("", vector<STRING>{"pn.varName", "r.varName"}, {"pn", "r"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_}, {"r", READ_} }, Clause("", vector<STRING>{"r.varName", "pn.varName"}, {"r", "pn"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator constant.value = integer") {
    setupWith();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CONSTANT_} }, Clause("", vector<STRING>{"c.value", "2"}, {"c"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"c", vector<int>{2}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = WithEvaluator::evaluate({ {"c", CONSTANT_} }, Clause("", vector<STRING>{"2", "c.value"}, {"c"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = { {"c", vector<int>{2}} };
    REQUIRE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CONSTANT_} }, Clause("", vector<STRING>{"c.value", "5"}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

}

TEST_CASE("With evaluator constant.value = s.stmt# (positive case)") {
    // case 1: no stmt num 290 in pkb
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CONSTANT_}, {"s", STMT_} }, Clause("", vector<STRING>{"c.value", "s.stmt#"}, {"c", "s"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["s"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(2, 2)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    // case 2: stmt num 290 in pkb
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(290, stmtNodeStub, ASSIGN_);
    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("2");
    PKB::constTable->storeConst("290");

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"c", CONSTANT_}, {"s", STMT_} }, Clause("", vector<STRING>{"c.value", "s.stmt#"}, {"c", "s"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["c"].at(i), tempResults2["s"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(2, 2), make_pair(290, 290)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator constant.value = s.stmt# (negative case)") {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(300, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("2");
    PKB::constTable->storeConst("290");
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CONSTANT_}, {"s", STMT_} }, Clause("", vector<STRING>{"c.value", "s.stmt#"}, {"c", "s"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);
}

TEST_CASE("With evaluator t.stmt# = integer / integer = t.stmt#") {
    setupWith();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"w", WHILE_} }, Clause("", vector<STRING>{"w.stmt#", "5"}, {"w"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { {"w", vector<int>{5}} };
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = WithEvaluator::evaluate({ {"ifs", IF_} }, Clause("", vector<STRING>{"10", "ifs.stmt#"}, {"ifs"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = { {"ifs", vector<int>{10}} };
    REQUIRE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"pn", PRINT_} }, Clause("", vector<STRING>{"pn.stmt#", "1"}, {"pn"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator t.stmt# = prog_line / prog_line = t.stmt# (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"c", CALL_}, {"n", PROGLINE_} }, Clause("", vector<STRING>{"c.stmt#", "n"}, {"c", "n"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["c"].at(i), tempResults1["n"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(4, 4), make_pair(9, 9)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"n", PROGLINE_}, {"r", READ_} }, Clause("", vector<STRING>{"n", "r.stmt#"}, {"n", "r"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["n"].at(i), tempResults2["r"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(13, 13), make_pair(8, 8)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator integer = prog_line / prog_line = integer (negative case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"n", PROGLINE_} }, Clause("", vector<STRING>{"15", "n"}, {"n"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"n", PROGLINE_}, {"r", READ_} }, Clause("", vector<STRING>{"n", "15"}, {"n"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator s.stmt# = t.stmt# / t.stmt# = s.stmt# (positive case)") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"s", STMT_}, {"w", WHILE_} }, Clause("", vector<STRING>{"s.stmt#", "w.stmt#"}, {"s", "w"}, 0), tempResults1);
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["s"].at(i), tempResults1["w"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = { {make_pair(5, 5)} };
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"s", STMT_}, {"ifs", IF_} }, Clause("", vector<STRING>{"ifs.stmt#", "s.stmt#"}, {"ifs", "s"}, 0), tempResults2);
    set<pair<int, int>> actual2;
    for (int i = 0; i < tempResults2.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults2["ifs"].at(i), tempResults2["s"].at(i));
        actual2.insert(p);
    }
    set<pair<int, int>> expected2 = { {make_pair(10, 10)} };
    REQUIRE(b2);
    REQUIRE(tempResults2.size() == 2);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("With evaluator s.stmt# = t.stmt# / t.stmt# = s.stmt# (negative case)") {
    PKB::stmtTable = new StmtTable();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"s", STMT_}, {"w", WHILE_} }, Clause("", vector<STRING>{"s.stmt#", "w.stmt#"}, {"s", "w"}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = { };
    REQUIRE_FALSE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = WithEvaluator::evaluate({ {"s", STMT_}, {"ifs", IF_} }, Clause("", vector<STRING>{"ifs.stmt#", "s.stmt#"}, {"ifs", "s"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = { };
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("With evaluator s.stmt# = s.stmt#") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"s", STMT_}, }, Clause("", vector<STRING>{"s.stmt#", "s.stmt#"}, {"s"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["s"].begin(), tempResults1["s"].end());
    unordered_set<int> expected1 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("With evaluator v.varName = v.varName") {
    setupWith();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = WithEvaluator::evaluate({ {"v", VARIABLE_}, }, Clause("", vector<STRING>{"v.varName", "v.varName"}, {"v"}, 0), tempResults1);
    unordered_set<int> actual1(tempResults1["v"].begin(), tempResults1["v"].end());
    unordered_set<int> expected1 {0, 1, 2, 3};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 1);
    REQUIRE(actual1 == expected1);
}
