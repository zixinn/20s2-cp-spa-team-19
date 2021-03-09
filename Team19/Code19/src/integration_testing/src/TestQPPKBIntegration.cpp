#include "PKB/PKB.h"
#include "PQL/QueryPreprocessor.h"
#include "PQL/QueryEvaluator.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

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

    string query2 = "stmt s; Select s";
    Query q2 = qp.process(query2);
    list<string> results2 = qe.evaluate(q2);
    unordered_set<string> actual2(begin(results2), end(results2));
    unordered_set<string> expected2 = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};
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
    unordered_set<string> expected3 = { "5", "6", "7", "8", "10", "12", "13", "14" };
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

/*TEST_CASE("QueryEvaluator evaluate query with multiple such that clauses") {
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
}*/

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
