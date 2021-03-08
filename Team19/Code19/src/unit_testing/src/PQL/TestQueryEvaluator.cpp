#include "PQL/QueryEvaluator.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
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
    PKB::uses->storeStmtUses(8, 2);
    PKB::uses->storeStmtUses(10, 0);
    PKB::uses->storeStmtUses(10, 1);
    PKB::uses->storeStmtUses(10, 2);
    PKB::uses->storeStmtUses(12, 1);
    PKB::uses->storeStmtUses(12, 0);
    PKB::uses->storeStmtUses(13, 2);
    PKB::uses->storeStmtUses(13, 0);
    PKB::uses->storeStmtUses(14, 1);
    PKB::uses->storeStmtUses(14, 2);
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
}

TEST_CASE("QueryEvaluator evaluate invalid query") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();
    Query q = Query({}, { "" }, {}, false, true);
    list<string> list = qe.evaluate(q);
    REQUIRE(list.empty());
}

TEST_CASE("QueryEvaluator evaluate query with no clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select v
    Query q = Query({ {"v", VARIABLE_} }, { "v" }, {}, true, true);
    list<string> list = qe.evaluate(q);
    unordered_set<string> actual(begin(list), end(list));
    unordered_set<string> expected = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual.size() == list.size());
    REQUIRE(actual == expected);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause and synonym in clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // stmt s; Select s such that Follows* (6, s)
    Clause c1 = Clause("Follows*", vector<string>{"6", "s"});
    Query q1 = Query({ {"s", STMT_} }, { "s" }, { c1 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "7", "8", "9" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; variable v; Select a such that Modifies (a, v)
    Clause c2 = Clause("Modifies", vector<string>{"a", "v"});
    Query q2 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { c2 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "1", "2", "3", "6", "7", "8", "11", "12", "13", "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause and synonym not in clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; while w1, w2; Select v such that Follows(w1, w2)
    Clause c1 = Clause("Follows", vector<string>{"w1", "w2"});
    Query q1 = Query({ {"v", VARIABLE_}, {"w1", WHILE_}, {"w2", WHILE_} }, { "v" }, { c1 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    REQUIRE(list1.empty());

    // variable v; while w; if ifs; Select v such that Follows(w, ifs)
    Clause c2 = Clause("Follows", vector<string>{"w", "ifs"});
    Query q2 = Query({ {"v", VARIABLE_}, {"w", WHILE_}, {"ifs", IF_} }, { "v" }, { c2 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one pattern clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; Select a pattern a(_, "count + 1")
    Clause c1 = Clause("a", vector<string>{"_", "\"count + 1\""});
    Query q1 = Query({ {"a", ASSIGN_} }, { "a" }, { c1 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "6" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; Select a pattern a("normSq", _"cenX * cenX"_)
    Clause c2 = Clause("a", vector<string>{"\"normSq\"", "_\"cenX * cenX\"_"});
    Query q2 = Query({ {"a", ASSIGN_} }, { "a" }, { c2 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; stmt s; Select v such that Follows* (6, s) such that Modifies (6, v)
    Clause c11 = Clause("Follows*", vector<string>{"6", "s"});
    Clause c12 = Clause("Modifies", vector<string>{"6", "v"});
    Query q1 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "v" }, { c11, c12 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "count" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // variable v; stmt s; Select v such that Follows* (5, s) such that Modifies (s, v)
    Clause c21 = Clause("Follows*", vector<string>{"5", "s"});
    Clause c22 = Clause("Modifies", vector<string>{"s", "v"});
    Query q2 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "v" }, { c21, c22 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "flag", "cenX", "cenY", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // stmt s1, s2; Select s1 such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, "cenY")
    Clause c31 = Clause("Follows", vector<string>{"s1", "s2"});
    Clause c32 = Clause("Follows*", vector<string>{"s1", "s2"});
    Clause c33 = Clause("Modifies", vector<string>{"s2", "\"cenY\""});
    Query q3 = Query({ {"s1", STMT_}, {"s2", STMT_} }, { "s1" }, { c31, c32, c33 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "2", "4", "5", "7", "12" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with one such that and one pattern clause") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) pattern a(v, _)
    Clause c11 = Clause("Uses", vector<string>{"a", "v"});
    Clause c12 = Clause("a", vector<string>{"v", "_"});
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { c11, c12 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "6", "7", "8", "12", "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _)
    Clause c21 = Clause("Parent*", vector<string>{"w", "a"});
    Clause c22 = Clause("a", vector<string>{"\"count\"", "_"});
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { c21, c22 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select BOOLEAN such that Uses(a, v)
    Clause c11 = Clause("Uses", vector<string>{"a", "v"});
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "BOOLEAN" }, { c11 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE"};
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // Select BOOLEAN such that Parent (5, 1)
    Clause c21 = Clause("Parent", vector<string>{"5", "1"});
    Query q2 = Query({ }, { "BOOLEAN" }, { c21 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    list<string> expected2 = { "FALSE" };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; stmt s; Select BOOLEAN such that Follows* (6, s) such that Modifies (6, v)
    Clause c11 = Clause("Follows*", vector<string>{"6", "s"});
    Clause c12 = Clause("Modifies", vector<string>{"6", "v"});
    Query q1 = Query({ {"v", VARIABLE_}, {"s", STMT_} }, { "BOOLEAN" }, { c11, c12 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    list<string> expected1 = { "TRUE" };
    REQUIRE(expected1.size() == list1.size());
    REQUIRE(expected1 == list1);

    // variable v; while w; ifs if; Select BOOLEAN such that Follows* (w, ifs) and Uses (ifs, v) and Uses (2, v)
    Clause c21 = Clause("Follows*", vector<string>{"w", "ifs"});
    Clause c22 = Clause("Uses", vector<string>{"ifs", "v"});
    Clause c23 = Clause("Uses", vector<string>{"2", "v"});
    Query q2 = Query({ {"v", VARIABLE_}, {"w", WHILE_}, {"ifs", IF_} }, { "BOOLEAN" }, { c21, c22, c23 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "FALSE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // stmt s1, s2; Select BOOLEAN such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, "cenY")
    Clause c31 = Clause("Follows", vector<string>{"s1", "s2"});
    Clause c32 = Clause("Follows*", vector<string>{"s1", "s2"});
    Clause c33 = Clause("Modifies", vector<string>{"s2", "\"cenY\""});
    Query q3 = Query({ {"s1", STMT_}, {"s2", STMT_} }, { "BOOLEAN" }, { c31, c32, c33 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "TRUE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple pattern clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; Select BOOLEAN pattern a(_, _"cenX*cenX"_) and a(_, _"cenY*cenY"_)
    Clause c11 = Clause("a", vector<string>{"_", "_\"cenX*cenX\"_"});
    Clause c12 = Clause("a", vector<string>{"_", "_\"cenY*cenY\"_"});
    Query q1 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { c11, c12 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; Select BOOLEAN pattern a("normSq", _"cenX*cenX"_) and a("normSq", _"cenY*cenY"_)
    Clause c21 = Clause("a", vector<string>{"\"normSq\"", "_\"cenX*cenX\"_"});
    Clause c22 = Clause("a", vector<string>{"\"normSq\"", "_\"cenY*cenY\"_"});
    Query q2 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { c21, c22 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; Select BOOLEAN pattern a(_, "cenX+cenX") and a(_, "cenY*cenY")
    Clause c31 = Clause("a", vector<string>{"_", "\"cenX+cenX\""});
    Clause c32 = Clause("a", vector<string>{"_", "\"cenY*cenY\""});
    Query q3 = Query({ {"a", ASSIGN_} }, { "BOOLEAN" }, { c31, c32 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) pattern a(v, _) such that Follows (12, a)
    Clause c11 = Clause("Uses", vector<string>{"a", "v"});
    Clause c12 = Clause("a", vector<string>{"v", "_"});
    Clause c13 = Clause("Follows", vector<string>{"12", "a"});
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { c11, c12, c13 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count + 1")
    Clause c21 = Clause("Parent*", vector<string>{"w", "a"});
    Clause c22 = Clause("a", vector<string>{"\"count\"", "_"});
    Clause c23 = Clause("a", vector<string>{"_", "\"count+1\""});
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { c21, c22, c23 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count")
    Clause c31 = Clause("Parent*", vector<string>{"w", "a"});
    Clause c32 = Clause("a", vector<string>{"\"count\"", "_"});
    Clause c33 = Clause("a", vector<string>{"_", "\"count\""});
    Query q3 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "w" }, { c31, c32, c33 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) pattern a(v, _) such that Follows (12, a)
    Clause c11 = Clause("Uses", vector<string>{"a", "v"});
    Clause c12 = Clause("a", vector<string>{"v", "_"});
    Clause c13 = Clause("Follows", vector<string>{"12", "a"});
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "BOOLEAN" }, { c11, c12, c13 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count + 1")
    Clause c21 = Clause("Parent*", vector<string>{"w", "a"});
    Clause c22 = Clause("a", vector<string>{"\"count\"", "_"});
    Clause c23 = Clause("a", vector<string>{"_", "\"count+1\""});
    Query q2 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "BOOLEAN" }, { c21, c22, c23 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    // assign a; while w; Select w such that Parent* (w, a) pattern a("count", _) and a(_, "count")
    Clause c31 = Clause("Parent*", vector<string>{"w", "a"});
    Clause c32 = Clause("a", vector<string>{"\"count\"", "_"});
    Clause c33 = Clause("a", vector<string>{"_", "\"count\""});
    Query q3 = Query({ {"a", ASSIGN_}, {"w", WHILE_} }, { "BOOLEAN" }, { c31, c32, c33 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause-semantically invalid") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select v such that Uses(_, v)
    Clause c11 = Clause("Uses", vector<string>{"_", "v"});
    Query q1 = Query({{"v", VARIABLE_} }, { "v" }, { c11 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // Select v such that Modifies (_, v)
    Clause c21 = Clause("Modifies", vector<string>{"_", "v"});
    Query q2 = Query({ {"v", VARIABLE_} }, { "v" }, { c21 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    list<string> expected2 = { };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause-semantically invalid, BOOLEAN") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // variable v; Select v such that Uses(_, v)
    Clause c11 = Clause("Uses", vector<string>{"_", "v"});
    Query q1 = Query({ {"v", VARIABLE_} }, { "BOOLEAN" }, { c11 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "FALSE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // Select v such that Modifies (_, v)
    Clause c21 = Clause("Modifies", vector<string>{"_", "v"});
    Query q2 = Query({ {"v", VARIABLE_} }, { "BOOLEAN" }, { c21 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    list<string> expected2 = {"FALSE" };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with two such that uses clauses") {
    setupQe();
    QueryEvaluator qe = QueryEvaluator();

    // assign a; variable v; Select a such that Uses(a, v) and Uses (a, "count")
    Clause c11 = Clause("Uses", vector<string>{"a", "v"});
    Clause c12 = Clause("Uses", vector<string>{"a", "\"count\""});
    Query q1 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a" }, { c11, c12 }, true, true);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "6", "12", "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    // assign a; variable v; Select v such that Uses(a, v) and Uses (a, "count")
    Query q2 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "v" }, { c11, c12 }, true, true);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "count", "cenX", "cenY" };
    REQUIRE(actual2 == expected2);
    REQUIRE(actual2.size() == list2.size());

    // assign a; variable v; if ifs; Select a such that Uses(a, v) and Uses (ifs, v)
    Clause c13 = Clause("Uses", vector<string>{"ifs", "v"});
    Query q3 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_}, {"ifs", IF_} }, { "a" }, { c11, c13 }, true, true);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "6", "7", "8", "12", "13", "14" };
    REQUIRE(actual3 == expected3);
    REQUIRE(actual3.size() == list3.size());

    // Select <a, v> such that Uses(a, v) and Uses (a, "count")
    /*Query q4 = Query({ {"a", ASSIGN_}, {"v", VARIABLE_} }, { "a", "v" }, { c11 }, true, true);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "6 count", "12 cenX", "12 count", "13 cenY", "13 count" };
    REQUIRE(actual4 == expected4);
    REQUIRE(actual4.size() == list4.size());*/
}
