#include "PKB/PKB.h"
#include "PQL/QueryPreprocessor.h"
#include "PQL/QueryEvaluator.h"
#include "catch.hpp"

using namespace std;

// Tests QP's correctness when processing queries
// and QE's ability to compute the correct results with the PKB's information

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

// Pre-populate the PKB
void setupQp() {
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

void setupQp2() {
    PKB::varTable = new VarTable();
    PKB::varTable->storeVarName("count"); // 0

    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("computeCentroid"); // 0

    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("0");
    PKB::constTable->storeConst("1");

    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);

    PKB::stmtTable->storeAssignExpr(5, "count", "0");
    PKB::stmtTable->storeAssignExpr(6, "cenX", "0");

    PKB::follows = new Follows();
    PKB::follows->storeFollows(1, 6);
    PKB::follows->populateFollowsStar();

    PKB::parent = new Parent();
    PKB::parent->storeParent(1, 2);
    PKB::parent->storeParent(2, 3);
    PKB::parent->storeParent(3, 4);
    PKB::parent->storeParent(4, 5);
    PKB::parent->populateParentStar();
}

TEST_CASE("QueryPreprocessor evaluate query with no clauses") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "variable v; Select v";
    Query q1 = qp.process(query1);
    list<string> results1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(results1), end(results1));
    unordered_set<string> expected1 = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual1.size() == results1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "prog_line n; Select n";
    Query q2 = qp.process(query2);
    list<string> results2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(results2), end(results2));
    unordered_set<string> expected2 = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
    REQUIRE(actual2.size() == results2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryPrepocessor evaluate query with one such that clause and synonym in clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "stmt s; Select s such that Follows*(6, s)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "7", "8", "9" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; variable v; Select a such that Modifies (a, v)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "1", "2", "3", "6", "7", "8", "11", "12", "13", "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "stmt s; variable v; Select    s such that     Uses (s, v)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "5", "6", "7", "8", "10", "12", "13", "14", "15", "16" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "assign assign; variable v; Select v such that Uses (assign, v)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "count", "cenX", "cenY", "y" , "x"};
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    string query5 = "stmt stmt; stmt stmt; Select stmt such that Follows (stmt, stmt)";
    Query q5 = qp.process(query5);
    REQUIRE_FALSE(q5.getIsSemanticallyValid());

    string query6 = "stmt stmt; Select stmt such that Parent* (10, stmt)";
    Query q6 = qp.process(query6);
    list<string> list6 = qe.evaluate(q6);
    unordered_set<string> actual6(begin(list6), end(list6));
    unordered_set<string> expected6 = { "11", "12", "13" };
    REQUIRE(actual6.size() == list6.size());
    REQUIRE(actual6 == expected6);

    string query7 = "assign stmt; Select stmt such that Uses (stmt, \"count\")";
    Query q7 = qp.process(query7);
    list<string> list7 = qe.evaluate(q7);
    unordered_set<string> actual7(begin(list7), end(list7));
    unordered_set<string> expected7 = { "6", "12", "13" };
    REQUIRE(actual7.size() == list7.size());
    REQUIRE(actual7 == expected7);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause and synonym not in clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "variable v; while w1, w2; Select v such that Follows(w1, w2)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    REQUIRE(list1.empty());

    string query2 = "variable v; while w; if ifs; Select v such that Follows(w, ifs)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "count", "cenX", "cenY", "x", "y", "flag", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one pattern clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; Select a        pattern a   (_, \"count + 1\")";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "6" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; Select a pattern a(\"normSq\", _\"cenX*cenX\"_)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "14" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; Select a pattern a (_, _\"count    +    1\"_)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "6" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "assign a; Select a pattern a(_, _\"(((count)))\"_   )   ";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "6", "12", "13" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    string query5 = "assign a; Select a pattern a(_, \"((((count))) + 1)\")";
    Query q5 = qp.process(query5);
    list<string> list5 = qe.evaluate(q5);
    unordered_set<string> actual5(begin(list5), end(list5));
    unordered_set<string> expected5 = { "6" };
    REQUIRE(actual5.size() == list5.size());
    REQUIRE(actual5 == expected5);
}

TEST_CASE("QueryPreprocessor evaluate query with one such that and one pattern clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; variable v; Select a such that Uses(a, v) pattern a(v, _)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "6", "7", "8", "12", "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; while w; Select w such that Parent* (w, a) pattern a(\"count\", _)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; variable v; Select a pattern a(v, _) such that Uses (a, v)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "6", "7", "8", "12", "13" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "variable v; stmt s; Select v such that Follows* (6, s) such that Modifies (6, v)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "count" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "variable v; stmt s; Select v such that Follows* (5, s) such that Modifies (s, v)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "flag", "cenX", "cenY", "normSq" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "stmt s1, s2; Select s1 such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, \"cenY\")";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "2", "4", "5", "7", "12" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query for nested statements") {
    setupQp2();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "stmt s; Select s such that Parent* (s, 4)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "1", "2", "3" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "stmt s; Select s such that Parent* (2, s)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "3", "4", "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "stmt s; Select s such that Parent (3, s)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "4" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "stmt s; Select s such that Parent (s, 5)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "4" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);
}

TEST_CASE("QueryEvaluator evaluate query for edge cases") {
    setupQp2();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "stmt s; Select s such that Parent* (14, s)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "stmt s; Select s such that Follows (14, s)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "stmt s; Select s such that Parent (s, 1)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "stmt s; Select s such that Follows* (s, 1)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);
}

TEST_CASE("QueryEvaluator evaluate ambiguous queries") {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "stmt BOOLEAN; Select BOOLEAN";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "FALSE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; Select <BOOLEAN, a>";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; stmt BOOLEAN; Select <BOOLEAN, a>";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = {"1 1", "1 2", "2 1", "2 2" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause BOOLEAN") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; variable v; Select BOOLEAN such that Uses(a, v)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE"};
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "Select BOOLEAN such that Parent (5, 1)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    list<string> expected2 = { "FALSE" };
    REQUIRE(list2.size() == expected2.size());
    REQUIRE(list2 == expected2);

    string query3 = "Select BOOLEAN such that Modifies (_, _)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    list<string> expected3 = { "FALSE" };
    REQUIRE(list3.size() == expected3.size());
    REQUIRE(list3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses BOOLEAN") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "variable v; stmt s; Select BOOLEAN such that Follows* (6, s) such that Modifies (6, v)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    list<string> expected1 = { "TRUE" };
    REQUIRE(expected1.size() == list1.size());
    REQUIRE(expected1 == list1);

    string query2 = "variable v; while w; if ifs; Select BOOLEAN such that Follows* (w, ifs) and Uses (ifs, v) and Uses (2, v)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "FALSE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "stmt s1, s2; Select BOOLEAN such that Follows (s1, s2) such that Follows* (s1, s2) such that Modifies (s2, \"cenY\")";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "TRUE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple pattern clauses BOOLEAN") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; Select BOOLEAN pattern a(_, _\"cenX*cenX\"_) and a(_, _\"cenY*cenY\"_)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; Select BOOLEAN pattern a(\"normSq\", _\"cenX*cenX\"_) and a(\"normSq\", _\"cenY*cenY\"_)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; Select BOOLEAN pattern a(_, \"cenX+cenX\") and a(_, \"cenY*cenY\")";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; variable v; Select a such that Uses(a, v) pattern a(v, _) such that Follows (12, a)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "13" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; while w; Select w such that Parent* (w, a) pattern a(\"count\", _) and a(_, \"count + 1\")";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; while w; Select w such that Parent* (w, a) pattern a(\"count\", _) and a(_, \"count\")";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses BOOLEAN") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v, _) such that Follows (12, a)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; while w; Select BOOLEAN such that Parent* (w, a) pattern a(\"count\", _) and a(_, \"count + 1\")";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "assign a; while w; Select BOOLEAN such that Parent* (w, a) pattern a(\"count\", _) and a(_, \"count\")";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with multiple such that and pattern clauses tuples") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a; variable v; Select <a, v> such that Uses(a, v) pattern a(v, _) such that Follows (12, a)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "13 cenY" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; while w; stmt s; Select <a, w, s> such that Parent* (w, a) and Follows* (7, s) pattern a(\"cenX\", _)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "7 5 8", "7 5 9" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with one such that clause-semantically invalid") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "variable v; Select v such that Uses(_, v)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "variable v; Select BOOLEAN such that Uses(_, v)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "FALSE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "variable v; Select v such that Modifies (_, v)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    list<string> expected3 = { };
    REQUIRE(list3.size() == expected3.size());
    REQUIRE(list3 == expected3);

    string query4 = "variable v; Select BOOLEAN such that Modifies (_, v)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    list<string> expected4 = {"FALSE" };
    REQUIRE(list4.size() == expected4.size());
    REQUIRE(list4 == expected4);
}

TEST_CASE("QueryEvaluator evaluate query with Calls/Calls* clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "procedure p1, p2; Select p2 such that Calls(\"computeCentroid\", p1) and Calls*(p1, p2)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "randomProcName" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "procedure p1, p2; Select p1 Calls(p1, p2) with p2.procName = \"computeCentriod\"";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with Next/Next* clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "prog_line n; Select n such that Next*(5, n) and Next (n, 5)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "9" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; variable v; Select <a, v> such that Uses(a, v) and Next*(4, a) and Next*(a, 10)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "6 count", "7 cenX", "7 x", "8 cenY", "8 y"};
    REQUIRE(actual2 == expected2);
    REQUIRE(actual2.size() == list2.size());
}

TEST_CASE("QueryEvaluator evaluate query with Affects/Affects* clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "assign a1, a2; Select <a1, a2> such that Affects(a1, a2)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "1 6", "1 12", "1 13", "2 7", "2 12", "2 14", "3 8", "3 13", "3 14", "6 6",
                                        "6 12", "6 13", "7 7", "7 12", "7 14", "8 8", "8 13", "8 14", "12 14", "13 14" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = " assign a; Select a such that Affects*(a, 8)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "3", "8" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryEvaluator evaluate query with if/while pattern matching clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "if ifs; while w; variable v; Select v pattern ifs(v,_,_) and w(v, _)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "x", "y" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "if ifs; while w; Select <ifs, w> pattern ifs(\"x\",_,_) and w(\"x\", _)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "10 5" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "if ifs; while w; Select <ifs, w> pattern ifs(\"flag\",_,_) and w(\"flag\", _)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);
}

TEST_CASE("QueryEvaluator evaluate query with 'with' clause") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "Select BOOLEAN with 12 = 12";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "TRUE" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; Select BOOLEAN with a.stmt# = 12";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "TRUE" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "read r; Select BOOLEAN with r.stmt# = 12";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "FALSE" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "assign a1, a; Select a1 with a.stmt# = 12";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "1", "2", "3", "6", "7", "8", "11", "12", "13", "14" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    string query5 = "assign a; read r; Select a with r.stmt# = 12";
    Query q5 = qp.process(query5);
    list<string> list5 = qe.evaluate(q5);
    unordered_set<string> actual5(begin(list5), end(list5));
    unordered_set<string> expected5 = { };
    REQUIRE(actual5.size() == list5.size());
    REQUIRE(actual5 == expected5);

    string query6 = "call c; Select c with \"readPoint\" = c.procName";
    Query q6 = qp.process(query6);
    list<string> list6 = qe.evaluate(q6);
    unordered_set<string> actual6(begin(list6), end(list6));
    unordered_set<string> expected6 = { "4" };
    REQUIRE(actual6.size() == list6.size());
    REQUIRE(actual6 == expected6);

    string query7 = "prog_line n; constant c; Select n with c.value = n";
    Query q7 = qp.process(query7);
    list<string> list7 = qe.evaluate(q7);
    unordered_set<string> actual7(begin(list7), end(list7));
    unordered_set<string> expected7 = { "1" };
    REQUIRE(actual7.size() == list7.size());
    REQUIRE(actual7 == expected7);
}

TEST_CASE("QueryEvaluator evaluate query with attribute in select") {
    setupQp();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "call c; prog_line n; Select c.stmt# such that Follows* (c, n)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "4" };
    REQUIRE(actual1.size() == list1.size());
    REQUIRE(actual1 == expected1);

    string query2 = "assign a; while w; Select a.stmt# such that Parent* (w, a) pattern w (_, _)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "6", "7", "8" };
    REQUIRE(actual2.size() == list2.size());
    REQUIRE(actual2 == expected2);

    string query3 = "constant c; assign a; Select c.value pattern a (_, _\"cenX\"_)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "0", "1" };
    REQUIRE(actual3.size() == list3.size());
    REQUIRE(actual3 == expected3);

    string query4 = "procedure p; variable v; if ifs; Select <p.procName, v.varName, ifs> pattern ifs (v, _, _) such that Uses (p, v)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "computeCentroid count 10", "computeCentroid x 10", "computeCentroid y 10", "computeCentroid cenY 10" };
    REQUIRE(actual4.size() == list4.size());
    REQUIRE(actual4 == expected4);

    string query5 = "call c; Select c.procName such that Affects (2, 7)";
    Query q5 = qp.process(query5);
    list<string> list5 = qe.evaluate(q5);
    unordered_set<string> actual5(begin(list5), end(list5));
    unordered_set<string> expected5 = { "readPoint", "randomProcName" };
    REQUIRE(actual5.size() == list5.size());
    REQUIRE(actual5 == expected5);

    string query6 = "print pn; variable v; Select <v, pn.varName> such that Uses (pn, v) and Next* (10, pn)";
    Query q6 = qp.process(query6);
    list<string> list6 = qe.evaluate(q6);
    unordered_set<string> actual6(begin(list6), end(list6));
    unordered_set<string> expected6 = { "cenX cenX", "cenY cenY" };
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

void setupQp3() {
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
    // PKB::affectsBip->setRunAffectsBip(true);
    PKB::populatePKB();
}

TEST_CASE("QueryEvaluator evaluate query with bip") {
    setupQp3();

    QueryPreprocessor qp = QueryPreprocessor();
    QueryEvaluator qe = QueryEvaluator();

    string query1 = "stmt s; Select s such that NextBip(1, s)";
    Query q1 = qp.process(query1);
    list<string> list1 = qe.evaluate(q1);
    unordered_set<string> actual1(begin(list1), end(list1));
    unordered_set<string> expected1 = { "4" };
    REQUIRE(actual1 == expected1);

    string query2 = "prog_line n; Select n such that NextBip*(n, n)";
    Query q2 = qp.process(query2);
    list<string> list2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(list2), end(list2));
    unordered_set<string> expected2 = { "4", "5", "6", "7" };
    REQUIRE(actual2 == expected2);

    /*string query3 = "assign a; Select a such that AffectsBip(a, 5)";
    Query q3 = qp.process(query3);
    list<string> list3 = qe.evaluate(q3);
    unordered_set<string> actual3(begin(list3), end(list3));
    unordered_set<string> expected3 = { "6" };
    REQUIRE(actual3 == expected3);

    string query4 = "Select BOOLEAN such that AffectsBip*(7, 4)";
    Query q4 = qp.process(query4);
    list<string> list4 = qe.evaluate(q4);
    unordered_set<string> actual4(begin(list4), end(list4));
    unordered_set<string> expected4 = { "FALSE" };
    REQUIRE(actual4 == expected4);*/
}
