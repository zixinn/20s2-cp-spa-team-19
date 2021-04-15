#include "PQL/QueryEvaluator.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index) : ast::Stmt(new sp::Token(), index) {};
};

void setupQe() {
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
    PKB::procTable->storeProcName("readPoint"); // 1
    PKB::procTable->storeProcName("randomProcName"); // 2

    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("0");
    PKB::constTable->storeConst("1");

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
    PKB::stmtTable->storeStmt(15, stmtNodeStub, PRINT_);
    PKB::stmtTable->storeStmt(16, stmtNodeStub, PRINT_);

    PKB::stmtTable->storeAssignExpr(1, "count", "(0)");
    PKB::stmtTable->storeAssignExpr(2, "cenX", "(0)");
    PKB::stmtTable->storeAssignExpr(3, "cenY", "(0)");
    PKB::stmtTable->storeAssignExpr(6, "count", "((count) + (1))");
    PKB::stmtTable->storeAssignExpr(7, "cenX", "((cenX) + (x))");
    PKB::stmtTable->storeAssignExpr(8, "cenY", "((cenY) + (y))");
    PKB::stmtTable->storeAssignExpr(11, "flag", "(1)");
    PKB::stmtTable->storeAssignExpr(12, "cenX", "((cenX) / (count))");
    PKB::stmtTable->storeAssignExpr(13, "cenY", "((cenY) / (count))");
    PKB::stmtTable->storeAssignExpr(14, "normSq", "(((cenX) * (cenX)) + ((cenY) * (cenY)))");

    PKB::stmtTable->storePrintVariableForStmt(15, 1);
    PKB::stmtTable->storePrintVariableForStmt(16, 2);

    PKB::stmtTable->storeIfPattern(10, 0);
    PKB::stmtTable->storeIfPattern(10, 3);
    PKB::stmtTable->storeIfPattern(10, 4);
    PKB::stmtTable->storeIfPattern(10, 2);
    PKB::stmtTable->storeWhilePattern(5, 5);
    PKB::stmtTable->storeWhilePattern(5, 3);
    PKB::stmtTable->storeWhilePattern(5, 4);

    PKB::follows = new Follows();
    PKB::follows->storeFollows(1, 2);
    PKB::follows->storeFollows(2, 3);
    PKB::follows->storeFollows(3, 4);
    PKB::follows->storeFollows(4, 5);
    PKB::follows->storeFollows(5, 10);
    PKB::follows->storeFollows(6, 7);
    PKB::follows->storeFollows(7, 8);
    PKB::follows->storeFollows(8, 9);
    PKB::follows->storeFollows(10, 14);
    PKB::follows->storeFollows(12, 13);
    PKB::follows->storeFollows(14, 15);
    PKB::follows->storeFollows(15, 16);
    PKB::follows->populateFollowsStar();

    PKB::parent = new Parent();
    PKB::parent->storeParent(5, 6);
    PKB::parent->storeParent(5, 7);
    PKB::parent->storeParent(5, 8);
    PKB::parent->storeParent(5, 9);
    PKB::parent->storeParent(10, 11);
    PKB::parent->storeParent(10, 12);
    PKB::parent->storeParent(10, 13);
    PKB::parent->populateParentStar();

    PKB::uses = new Uses();
    PKB::uses->storeStmtUses(5, 3);
    PKB::uses->storeStmtUses(5, 4);
    PKB::uses->storeStmtUses(5, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(5, 2);
    PKB::uses->storeStmtUses(6, 0);
    PKB::uses->storeStmtUses(7, 1);
    PKB::uses->storeStmtUses(7, 3);
    PKB::uses->storeStmtUses(8, 2);
    PKB::uses->storeStmtUses(8, 4);
    PKB::uses->storeStmtUses(10, 0);
    PKB::uses->storeStmtUses(10, 1);
    PKB::uses->storeStmtUses(10, 2);
    PKB::uses->storeStmtUses(12, 1);
    PKB::uses->storeStmtUses(12, 0);
    PKB::uses->storeStmtUses(13, 2);
    PKB::uses->storeStmtUses(13, 0);
    PKB::uses->storeStmtUses(14, 1);
    PKB::uses->storeStmtUses(14, 2);
    PKB::uses->storeStmtUses(15, 1);
    PKB::uses->storeStmtUses(16, 2);
    PKB::uses->storeProcUses(0, 0);
    PKB::uses->storeProcUses(0, 1);
    PKB::uses->storeProcUses(0, 2);
    PKB::uses->storeProcUses(0, 3);
    PKB::uses->storeProcUses(0, 4);

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

    PKB::next = new Next();
    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(7, 8);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(9, 5);
    PKB::next->storeNext(5, 10);
    PKB::next->storeNext(10, 11);
    PKB::next->storeNext(10, 12);
    PKB::next->storeNext(12, 13);
    PKB::next->storeNext(13, 14);
    PKB::next->storeNext(11, 14);
    PKB::next->storeNext(14, 15);
    PKB::next->storeNext(15, 16);
    PKB::next->populateNextStar();

    PKB::calls = new Calls();
    PKB::calls->storeCalls(4, 0, 1);
    PKB::calls->storeCalls(9, 1, 2);
    PKB::calls->processCalls();

    PKB::affects = new Affects();
    PKB::affects->populateAffectsAndAffectsStar();
}

TEST_CASE("QueryEvaluator evaluate invalid query") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();
    Query q = Query({}, { "" }, {{}}, false, true);
    list<STRING> list = qe.evaluate(q);
    REQUIRE(list.empty());
}

TEST_CASE("QueryEvaluator evaluate query with no clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select v
    Query q1 = Query({ {"v", VARIABLE_} }, { "v" }, {{}}, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // prog_line n; Select n
    Query q2 = Query({ {"n", PROGLINE_} }, { "n" }, {{}}, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause and synonym in clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // stmt s; Select s such that Follows* (6, s)
    Clause c1 = Clause("Follows*", vector<STRING>{"6", "s"}, {"s"}, 1);
    Query q1 = Query({ {"s", STMT_} }, { "s" }, { {c1} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "7", "8", "9" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; variable v; Select a such that Modifies (a, v)
    Clause c2 = Clause("Modifies", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Query q2 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { {c2} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "1", "2", "3", "6", "7", "8", "11", "12", "13", "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause and synonym not in clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; while w1, w2; Select v such that Follows(w1, w2)
    Clause c1 = Clause("Follows", vector<STRING>{"w1", "w2"}, {"w1", "w2"}, 0);
    Query q1 = Query({ {"v", VARIABLE_}, {"w1", WHILE_}, {"w2", WHILE_} }, { "v" }, { {c1} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    REQUIRE(list1.empty());

    // variable v; while w; if ifs; Select v such that Follows(w, ifs)
    Clause c2 = Clause("Follows", vector<STRING>{"w", "ifs"}, {"w", "ifs"}, 0);
    Query q2 = Query({ {"v", VARIABLE_}, {"w", WHILE_}, {"ifs", IF_} }, { "v" }, { {c2} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one pattern clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; Select a pattern a(_, "count + 1")
    Clause c1 = Clause("a", vector<STRING>{"_", "\"count + 1\""}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_} }, { "a" }, { {c1} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "6" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; Select a pattern a("normSq", _"cenX * cenX"_)
    Clause c2 = Clause("a", vector<STRING>{"\"normSq\"", "_\"cenX * cenX\"_"}, {"a"}, 2);
    Query q2 = Query({ {"a", ASSIGN_} }, { "a" }, { {c2} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; stmt s; Select v such that Follows* (6, s) such that Modifies (6, v)
    Clause c11 = Clause("Follows*", vector<STRING>{"6", "s"}, {"s"}, 1);
    Clause c12 = Clause("Modifies", vector<STRING>{"6", "v"}, {"v"}, 1);
    Query q1 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "v" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "count" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // variable v; stmt s; Select v such that Follows* (5, s) such that Modifies (s, v)
    Clause c21 = Clause("Follows*", vector<STRING>{"5", "s"}, {"s"}, 1);
    Clause c22 = Clause("Modifies", vector<STRING>{"s", "v"}, {"s", "v"}, 0);
    Query q2 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "v" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "flag", "cenX", "cenY", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // stmt s1, s2; Select s1 such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, "cenY")
    Clause c31 = Clause("Follows", vector<STRING>{"s1", "s2"}, {"s1", "s2"}, 0);
    Clause c32 = Clause("Follows*", vector<STRING>{"s1", "s2"}, {"s1", "s2"}, 0);
    Clause c33 = Clause("Modifies", vector<STRING>{"s2", "\"cenY\""}, {"s2"}, 1);
    Query q3 = Query({ {"s1", STMT_}, {"s2", STMT_} }, { "s1" }, { {c31, c32, c33} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "2", "4", "5", "7", "12" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with one such that and one pattern clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) pattern a(v, _)
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c12 = Clause("a", vector<STRING>{"v", "_"}, {"a", "v"}, 0);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "6", "7", "8", "12", "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _)
    Clause c21 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c22 = Clause("a", vector<STRING>{"\"count\"", "_"}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select BOOLEAN such that Uses(a, v)
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "BOOLEAN" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "TRUE"};
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // Select BOOLEAN such that Parent (5, 1)
    Clause c21 = Clause("Parent", vector<STRING>{"5", "1"}, {}, 2);
    Query q2 = Query({ }, { "BOOLEAN" }, { {c21} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    list<STRING> expected2 = { "FALSE" };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);

    // Select BOOLEAN such that Modifies (_, _)
    Query q3 = Query({}, { "BOOLEAN" }, {{}}, true, false);
    list<STRING> list3 = qe.evaluate(q3);
    list<STRING> expected3 = { "FALSE" };
    REQUIRE(list3.size() == expected3.size());
    REQUIRE(list3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; stmt s; Select BOOLEAN such that Follows* (6, s) such that Modifies (6, v)
    Clause c11 = Clause("Follows*", vector<STRING>{"6", "s"}, {"s"}, 1);
    Clause c12 = Clause("Modifies", vector<STRING>{"6", "v"}, {"v"}, 1);
    Query q1 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "BOOLEAN" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    list<STRING> expected1 = { "TRUE" };
    REQUIRE(expected1.size() == list1.size());
    REQUIRE(expected1 == list1);

    // variable v; while w; if ifs; Select BOOLEAN such that Follows* (w, ifs) and Uses (ifs, v) and Uses (2, v)
    Clause c21 = Clause("Follows*", vector<STRING>{"w", "ifs"}, {"w", "ifs"}, 0);
    Clause c22 = Clause("Uses", vector<STRING>{"ifs", "v"}, {"ifs", "v"}, 0);
    Clause c23 = Clause("Uses", vector<STRING>{"2", "v"}, {"v"}, 1);
    Query q2 = Query({ {"v", VARIABLE_}, {"w", WHILE_}, {"ifs", IF_} }, { "BOOLEAN" }, { {c21, c22, c23} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "FALSE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // stmt s1, s2; Select BOOLEAN such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, "cenY")
    Clause c31 = Clause("Follows", vector<STRING>{"s1", "s2"}, {"s1", "s2"}, 0);
    Clause c32 = Clause("Follows*", vector<STRING>{"s1", "s2"}, {"s1", "s2"}, 0);
    Clause c33 = Clause("Modifies", vector<STRING>{"s2", "\"cenY\""}, {"s2"}, 1);
    Query q3 = Query({ {"s1", STMT_}, {"s2", STMT_} }, { "BOOLEAN" }, { {c31, c32, c33} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "TRUE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple pattern clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; Select BOOLEAN pattern a(_, _"cenX*cenX"_) and a(_, _"cenY*cenY"_)
    Clause c11 = Clause("a", vector<STRING>{"_", "_\"cenX*cenX\"_"}, {"a"}, 1);
    Clause c12 = Clause("a", vector<STRING>{"_", "_\"cenY*cenY\"_"}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; Select BOOLEAN pattern a("normSq", _"cenX*cenX"_) and a("normSq", _"cenY*cenY"_)
    Clause c21 = Clause("a", vector<STRING>{"\"normSq\"", "_\"cenX*cenX\"_"}, {"a"}, 2);
    Clause c22 = Clause("a", vector<STRING>{"\"normSq\"", "_\"cenY*cenY\"_"}, {"a"}, 2);
    Query q2 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; Select BOOLEAN pattern a(_, "cenX+cenX") and a(_, "cenY*cenY")
    Clause c31 = Clause("a", vector<STRING>{"_", "\"cenX+cenX\""}, {"a"}, 1);
    Clause c32 = Clause("a", vector<STRING>{"_", "\"cenY*cenY\""}, {"a"}, 1);
    Query q3 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { {c31, c32} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) pattern a(v, _) such that Follows (12, a)
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c12 = Clause("a", vector<STRING>{"v", "_"}, {"a", "v"}, 0);
    Clause c13 = Clause("Follows", vector<STRING>{"12", "a"}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { {c11, c12, c13} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count + 1")
    Clause c21 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c22 = Clause("a", vector<STRING>{"\"count\"", "_"}, {"a"}, 1);
    Clause c23 = Clause("a", vector<STRING>{"_", "\"count+1\""}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { {c21, c22, c23} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count")
    Clause c31 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c32 = Clause("a", vector<STRING>{"\"count\"", "_"}, {"a"}, 1);
    Clause c33 = Clause("a", vector<STRING>{"_", "\"count\""}, {"a"}, 1);
    Query q3 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { {c31, c32, c33} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v, _) such that Follows (12, a)
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c12 = Clause("a", vector<STRING>{"v", "_"}, {"a", "v"}, 0);
    Clause c13 = Clause("Follows", vector<STRING>{"12", "a"}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "BOOLEAN" }, { {c11, c12, c13} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select BOOLEAN such that Parent* (w, a) pattern a("count", _) and a(_, "count + 1")
    Clause c21 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c22 = Clause("a", vector<STRING>{"\"count\"", "_"}, {"a"}, 1);
    Clause c23 = Clause("a", vector<STRING>{"_", "\"count+1\""}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "BOOLEAN" }, { {c21, c22, c23} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; while w; Select BOOLEAN such that Parent* (w, a) pattern a("count", _) and a(_, "count")
    Clause c31 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c32 = Clause("a", vector<STRING>{"\"count\"", "_"}, {"a"}, 1);
    Clause c33 = Clause("a", vector<STRING>{"_", "\"count\""}, {"a"}, 1);
    Query q3 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "BOOLEAN" }, { {c31, c32, c33} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses tuples") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select <a, v> such that Uses(a, v) pattern a(v, _) such that Follows (12, a)
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c12 = Clause("a", vector<STRING>{"v", "_"}, {"a", "v"}, 0);
    Clause c13 = Clause("Follows", vector<STRING>{"12", "a"}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a", "v" }, { {c11, c12, c13} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "13 cenY" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; stmt s; Select <a, w, s> such that Parent* (w, a) and Follows* (7, s) pattern a("cenX", _)
    Clause c21 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c22 = Clause("Follows*", vector<STRING>{"7", "s"}, {"s"}, 1);
    Clause c23 = Clause("a", vector<STRING>{"\"cenX\"", "_"}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_}, {"s", STMT_} }, { "a", "w", "s" }, { {c21, c22, c23} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "7 5 8", "7 5 9" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause-semantically invalid") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select v such that Uses(_, v)
    Clause c11 = Clause("Uses", vector<STRING>{"_", "v"}, {"v"}, 0);
    Query q1 = Query({{"v", VARIABLE_} }, { "v" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // variable v; Select v such that Modifies (_, v)
    Clause c21 = Clause("Modifies", vector<STRING>{"_", "v"}, {"v"}, 0);
    Query q2 = Query({ {"v", VARIABLE_} }, { "v" }, { {c21} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    list<STRING> expected2 = { };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause-semantically invalid, BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select BOOLEAN such that Uses(_, v)
    Clause c11 = Clause("Uses", vector<STRING>{"_", "v"}, {"v"}, 0);
    Query q1 = Query({ {"v", VARIABLE_} }, { "BOOLEAN" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "FALSE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // variable v; Select BOOLEAN such that Modifies (_, v)
    Clause c21 = Clause("Modifies", vector<STRING>{"_", "v"}, {"v"}, 0);
    Query q2 = Query({ {"v", VARIABLE_} }, { "BOOLEAN" }, { {c21} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    list<STRING> expected2 = {"FALSE" };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with two such that uses clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) and Uses (a, "count")
    Clause c11 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c12 = Clause("Uses", vector<STRING>{"a", "\"count\""}, {"a"}, 1);
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "6", "12", "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; variable v; Select v such that Uses(a, v) and Uses (a, "count")
    Query q2 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "v" }, { {c11, c12} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "count", "cenX", "cenY" };
    REQUIRE(actual2 == expected2);
    REQUIRE(actual2.size() == list2.size());

    // assign a; variable v; if ifs; Select a such that Uses(a, v) and Uses (ifs, v)
    Clause c13 = Clause("Uses", vector<STRING>{"ifs", "v"}, {"ifs", "v"}, 0);
    Query q3 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_}, {"ifs", IF_} }, { "a" }, { {c11, c13} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "6", "7", "8", "12", "13", "14" };
    REQUIRE(actual3 == expected3);
    REQUIRE(actual3.size() == list3.size());

    // Select <a, v> such that Uses(a, v) and Uses (a, "count")
    Query q4 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a", "v" }, { {c11, c12} }, true, true);
    list<STRING> list4 = qe.evaluate(q4);
    unordered_set<STRING> actual4(begin(list4), end(list4));
    unordered_set<STRING> expected4 = { "6 count", "12 cenX", "12 count", "13 cenY", "13 count" };
    REQUIRE(actual4 == expected4);
    REQUIRE(actual4.size() == list4.size());
}

TEST_CASE("QueryEvaluator evaluate query with Calls/Calls* clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // procedure p1, p2; Select p2 such that Calls("computeCentroid", p1) and Calls*(p1, p2)
    Clause c11 = Clause("Calls", vector<STRING>{"\"computeCentroid\"", "p1"}, {"p1"}, 1);
    Clause c12 = Clause("Calls*", vector<STRING>{"p1", "p2"}, {"p1", "p2"}, 0);
    Query q1 = Query({ {"p1", PROCEDURE_}, {"p2", PROCEDURE_} }, { "p2" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "randomProcName" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // procedure p1, p2; Select p1 Calls(p1, p2) with p2.procName = "computeCentriod"
    Clause c21 = Clause("Calls", vector<STRING>{"p1", "p2"}, {"p1", "p2"}, 0);
    Clause c22 = Clause("", vector<STRING>{"p2.procName", "\"computeCentroid\""}, {"p2"}, 1);
    Query q2 = Query({ {"p1", PROCEDURE_}, {"p2", PROCEDURE_} }, { "p1" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with Next/Next* clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();
    // prog_line n; Select n such that Next*(5, n) and Next (n, 5)
    Clause c11 = Clause("Next*", vector<STRING>{"5", "n"}, {"n"}, 1);
    Clause c12 = Clause("Next", vector<STRING>{"n", "5"}, {"n"}, 1);
    Query q1 = Query({ {"n", PROGLINE_} }, { "n" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "9" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; variable v; Select <a, v> such that Uses(a, v) and Next*(4, a) and Next*(a, 10)
    Clause c21 = Clause("Uses", vector<STRING>{"a", "v"}, {"a", "v"}, 0);
    Clause c22 = Clause("Next*", vector<STRING>{"4", "a"}, {"a"}, 1);
    Clause c23 = Clause("Next*", vector<STRING>{"a", "10"}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a", "v" }, { {c21, c22, c23} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "6 count", "7 cenX", "7 x", "8 cenY", "8 y"};
    REQUIRE(actual2 == expected2);
    REQUIRE(actual2.size() == list2.size());
}

TEST_CASE("QueryEvaluator evaluate query with Affects/Affects* clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a1, a2; Select <a1, a2> such that Affects(a1, a2)
    Clause c11 = Clause("Affects", vector<STRING>{"a1", "a2"}, {"a1", "a2"}, 0);
    Query q1 = Query({ {"a1", ASSIGN_}, {"a2", ASSIGN_} }, { "a1", "a2" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "1 6", "1 12", "1 13", "2 7", "2 12", "2 14", "3 8", "3 13", "3 14", "6 6",
                                        "6 12", "6 13", "7 7", "7 12", "7 14", "8 8", "8 13", "8 14", "12 14", "13 14" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; Select a such that Affects*(a, 8)
    Clause c21 = Clause("Affects*", vector<STRING>{"a", "8"}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_} }, { "a" }, { {c21} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "3", "8" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with if/while pattern matching clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // if ifs; while w; variable v; Select v pattern ifs(v,_,_) and w(v, _)
    Clause c11 = Clause("ifs", vector<STRING>{"v", "_", "_"}, {"ifs", "v"}, 0);
    Clause c12 = Clause("w", vector<STRING>{"v", "_"}, {"w", "v"}, 0);
    Query q1 = Query({ {"v", VARIABLE_}, {"ifs", IF_}, {"w", WHILE_} }, { "v" }, { {c11, c12} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "x", "y" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // if ifs; while w; Select <ifs, w> pattern ifs("x",_,_) and w("x", _)
    Clause c21 = Clause("ifs", vector<STRING>{"\"x\"", "_", "_"}, {"ifs"}, 1);
    Clause c22 = Clause("w", vector<STRING>{"\"x\"", "_"}, {"w"}, 1);
    Query q2 = Query({ {"ifs", IF_}, {"w", WHILE_} }, { "ifs", "w" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "10 5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // if ifs; while w; Select <ifs, w> pattern ifs("flag",_,_) and w("flag", _)
    Clause c31 = Clause("ifs", vector<STRING>{"\"flag\"", "_", "_"}, {"ifs"}, 1);
    Clause c32 = Clause("w", vector<STRING>{"\"flag\"", "_"}, {"w"}, 1);
    Query q3 = Query({ {"ifs", IF_}, {"w", WHILE_} }, { "ifs", "w" }, { {c31, c32} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with 'with' clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    //Select BOOLEAN with 12 = 12
    Clause c11 = Clause("", vector<STRING>{"12", "12"}, {}, 2);
    Query q1 = Query({ }, { "BOOLEAN" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    //assign a; Select BOOLEAN with a.stmt# = 12
    Clause c21 = Clause("", vector<STRING>{"a.stmt#", "12"}, {"a"}, 1);
    Query q2 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { {c21} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    //read r; Select BOOLEAN with r.stmt# = 12
    Clause c31 = Clause("", vector<STRING>{"r.stmt#", "12"}, {"r"}, 1);
    Query q3 = Query({ {"r", READ_} }, { "BOOLEAN" }, { {c31} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    // assign a1, a; Select a1 with a.stmt# = 12
    Clause c41 = Clause("", vector<STRING>{"a.stmt#", "12"}, {"a"}, 1);
    Query q4 = Query({ {"a", ASSIGN_}, {"a1", ASSIGN_ } }, { "a1" }, { {c41} }, true, true);
    list<STRING> list4 = qe.evaluate(q4);
    unordered_set<STRING> actual4(begin(list4), end(list4));
    unordered_set<STRING> expected4 = { "1", "2", "3", "6", "7", "8", "11", "12", "13", "14" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    // assign a; read r; Select a with r.stmt# = 12
    Clause c51 = Clause("", vector<STRING>{"r.stmt#", "12"}, {"r"}, 1);
    Query q5 = Query({ {"a", ASSIGN_}, {"r", READ_ } }, { "a" }, { {c51} }, true, true);
    list<STRING> list5 = qe.evaluate(q5);
    unordered_set<STRING> actual5(begin(list5), end(list5));
    unordered_set<STRING> expected5 = { };
    REQUIRE(actual5.size() == list5.size());
    REQUIRE(actual5 == expected5);

    // call c; Select c with "readPoint" = c.procName
    Clause c61 = Clause("", vector<STRING>{"\"readPoint\"", "c.procName"}, {"c"}, 1);
    Query q6 = Query({ {"c", CALL_} }, { "c" }, { {c61} }, true, true);
    list<STRING> list6 = qe.evaluate(q6);
    unordered_set<STRING> actual6(begin(list6), end(list6));
    unordered_set<STRING> expected6 = { "4" };
    REQUIRE(actual6.size() == list6.size());
    REQUIRE(actual6 == expected6);

    // prog_line n; constant c; Select n with c.value = n
    Clause c71 = Clause("", vector<STRING>{"c.value", "n"}, {"c", "n"}, 0);
    Query q7 = Query({ {"n", PROGLINE_} }, { "n" }, { {c71} }, true, true);
    list<STRING> list7 = qe.evaluate(q7);
    unordered_set<STRING> actual7(begin(list7), end(list7));
    unordered_set<STRING> expected7 = { "1" };
    REQUIRE(actual7.size() == list7.size());
    REQUIRE(actual7 == expected7);
}

TEST_CASE("QueryEvaluator evaluate query with attribute in select") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // call c; prog_line n; Select c.stmt# such that Follows* (c, n)
    Clause c11 = Clause("Follows*", vector<STRING>{"c", "n"}, {"c", "n"}, 0);
    Query q1 = Query({ {"c", CALL_}, {"n", PROGLINE_} }, { "c.stmt#" }, { {c11} }, true, true);
    list<STRING> list1 = qe.evaluate(q1);
    unordered_set<STRING> actual1(begin(list1), end(list1));
    unordered_set<STRING> expected1 = { "4" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select a.stmt# such that Parent* (w, a) pattern w (_, _)
    Clause c21 = Clause("Parent*", vector<STRING>{"w", "a"}, {"w", "a"}, 0);
    Clause c22 = Clause("w", vector<STRING>{"_", "_"}, {"w"}, 0);
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "a.stmt#" }, { {c21, c22} }, true, true);
    list<STRING> list2 = qe.evaluate(q2);
    unordered_set<STRING> actual2(begin(list2), end(list2));
    unordered_set<STRING> expected2 = { "6", "7", "8" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // constant c; assign a; Select c.value pattern a (_, _"cenX"_)
    Clause c31 = Clause("a", vector<STRING>{"_", "_\"cenX\"_"}, {"a"}, 1);
    Query q3 = Query({ {"c", CONSTANT_}, {"a", ASSIGN_} }, { "c.value" }, { {c31} }, true, true);
    list<STRING> list3 = qe.evaluate(q3);
    unordered_set<STRING> actual3(begin(list3), end(list3));
    unordered_set<STRING> expected3 = { "0", "1" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    // procedure p; variable v; if ifs; Select <p.procName, v.varName, ifs> pattern ifs (v, _, _) such that Uses (p, v)
    Clause c41 = Clause("ifs", vector<STRING>{"v", "_", "_"}, {"ifs", "v"}, 0);
    Clause c42 = Clause("Uses", vector<STRING>{"p", "v"}, {"p", "v"}, 0);
    Query q4 = Query({ {"p", PROCEDURE_}, {"v", VARIABLE_}, {"ifs", IF_} }, { "p.procName", "v.varName", "ifs" }, { {c41, c42} }, true, true);
    list<STRING> list4 = qe.evaluate(q4);
    unordered_set<STRING> actual4(begin(list4), end(list4));
    unordered_set<STRING> expected4 = { "computeCentroid count 10", "computeCentroid x 10", "computeCentroid y 10", "computeCentroid cenY 10" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    // call c; Select c.procName such that Affects (2, 7)
    Clause c51 = Clause("Affects", vector<STRING>{"2", "7"}, {}, 2);
    Query q5 = Query({ {"c", CALL_} }, { "c.procName" }, { {c51} }, true, true);
    list<STRING> list5 = qe.evaluate(q5);
    unordered_set<STRING> actual5(begin(list5), end(list5));
    unordered_set<STRING> expected5 = { "readPoint", "randomProcName" };
    REQUIRE(actual5.size() == list5.size());
    REQUIRE(actual5 == expected5);

    // print pn; variable v; Select <v, pn.varName> such that Uses (pn, v) and Next* (10, pn)
    Clause c61 = Clause("Uses", vector<STRING>{ "pn", "v" }, {"pn", "v"}, 0);
    Clause c62 = Clause("Next*", vector<STRING>{ "10", "pn" }, {"pn"}, 1);
    Query q6 = Query({ {"pn", PRINT_}, {"v", VARIABLE_} }, { "v", "pn.varName" }, { {c61, c62} }, true, true);
    list<STRING> list6 = qe.evaluate(q6);
    unordered_set<STRING> actual6(begin(list6), end(list6));
    unordered_set<STRING> expected6 = { "cenX cenX", "cenY cenY" };
    REQUIRE(actual6.size() == list6.size());
    REQUIRE(actual6 == expected6);
}

//    procedure B {
//    01      call C;
//    02      call C;
//    03      call C; }
//
//    procedure C {
//    04      d = a;
//    05      a = b;
//    06      b = c;
//    07      c = d; }

void setupQe2() {
    PKB::resetPKB();

    PKB::varTable->storeVarName("d"); // 0
    PKB::varTable->storeVarName("a"); // 1
    PKB::varTable->storeVarName("b"); // 2
    PKB::varTable->storeVarName("c"); // 3

    PKB::procTable->storeProcName("B"); // 0
    PKB::procTable->storeProcName("C"); // 1
    PKB::procTable->storeProcStmt(0, 1, 3);
    PKB::procTable->storeProcStmt(1, 4, 7);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);

    PKB::uses->storeStmtUses(4, 1);
    PKB::uses->storeStmtUses(5, 2);
    PKB::uses->storeStmtUses(6, 3);
    PKB::uses->storeStmtUses(7, 0);
    PKB::uses->storeProcUses(1, 1);
    PKB::uses->storeProcUses(1, 2);
    PKB::uses->storeProcUses(1, 3);
    PKB::uses->storeProcUses(1, 0);

    PKB::modifies->storeStmtModifies(4, 0);
    PKB::modifies->storeStmtModifies(5, 1);
    PKB::modifies->storeStmtModifies(6, 2);
    PKB::modifies->storeStmtModifies(7, 3);
    PKB::modifies->storeProcModifies(1, 0);
    PKB::modifies->storeProcModifies(1, 1);
    PKB::modifies->storeProcModifies(1, 2);
    PKB::modifies->storeProcModifies(1, 3);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);

    PKB::calls->storeCalls(1, 0, 1);
    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(3, 0, 1);

    PKB::nextBip->setRunNextBip(true);
    PKB::affectsBip->setRunAffectsBip(true);
    PKB::populatePKB();
}