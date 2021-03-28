#include "PQL/QueryOptimizer.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
};

//    procedure sumDigits {
//    1.  read number;
//    2.  sum = 0;
//    3.  while (number > 0) {
//    4.      digit = number % 10;
//    5.      sum = sum + digit;
//    6.      number = number / 10;
//        }
//    7.  print sum;
//    }

void setUpQo() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);

    PKB::procTable->storeProcName("sumDigits"); // 0

    PKB::varTable->storeVarName("number"); // 0
    PKB::varTable->storeVarName("sum"); // 1
    PKB::varTable->storeVarName("digit"); // 2

    PKB::constTable->storeConst("0");
    PKB::constTable->storeConst("10");

    PKB::stmtTable->storeStmt(1, stmtNodeStub, READ_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, PRINT_);

    PKB::follows->storeFollows(1, 2);
    PKB::follows->storeFollows(2, 3);
    PKB::follows->storeFollows(4, 5);
    PKB::follows->storeFollows(5, 6);
    PKB::follows->storeFollows(3, 7);

    PKB::parent->storeParent(3, 4);
    PKB::parent->storeParent(3, 5);
    PKB::parent->storeParent(3, 6);

    PKB::uses->storeStmtUses(3, 0);
    PKB::uses->storeStmtUses(3, 1);
    PKB::uses->storeStmtUses(3, 2);
    PKB::uses->storeStmtUses(4, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(5, 2);
    PKB::uses->storeStmtUses(6, 0);
    PKB::uses->storeStmtUses(7, 1);
    PKB::uses->storeProcUses(0, 0);
    PKB::uses->storeProcUses(0, 1);
    PKB::uses->storeProcUses(0, 2);

    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(2, 1);
    PKB::modifies->storeStmtModifies(3, 0);
    PKB::modifies->storeStmtModifies(3, 1);
    PKB::modifies->storeStmtModifies(3, 2);
    PKB::modifies->storeStmtModifies(4, 2);
    PKB::modifies->storeStmtModifies(5, 1);
    PKB::modifies->storeStmtModifies(6, 0);
    PKB::modifies->storeProcModifies(0, 1);
    PKB::modifies->storeProcModifies(0, 2);
    PKB::modifies->storeProcModifies(0, 3);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(3, 7);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(6, 3);

    PKB::populatePKB();
}

TEST_CASE("QueryOptimizer test setUpQo") {
    setUpQo();
    REQUIRE(PKB::follows->getFollowsSize() == 5);
    REQUIRE(PKB::follows->getFollowsStarSize() == 9);
    REQUIRE(PKB::parent->getParentSize() == 3);
    REQUIRE(PKB::parent->getParentStarSize() == 3);
    REQUIRE(PKB::uses->getProcSize() == 3);
    REQUIRE(PKB::uses->getStmtSize() == 8);
    REQUIRE(PKB::modifies->getProcSize() == 3);
    REQUIRE(PKB::modifies->getStmtSize() == 8);
    REQUIRE(PKB::calls->getCallsSize() == 0);
    REQUIRE(PKB::calls->getCallsStarSize() == 0);
    REQUIRE(PKB::next->getNextSize() == 8);
    REQUIRE(PKB::next->getNextStarSize() == 31);
    REQUIRE(PKB::affects->getAffectsSize() == 5);
    REQUIRE(PKB::affects->getAffectsStarSize() == 6);
    REQUIRE(PKB::procTable->getSize() == 1);
    REQUIRE(PKB::varTable->getSize() == 3);
    REQUIRE(PKB::constTable->getSize() == 2);
    REQUIRE(PKB::stmtTable->getSize() == 7);
    REQUIRE(PKB::stmtTable->getAllAssignStmtNums().size() == 4);
    REQUIRE(PKB::stmtTable->getAllWhileStmtNums().size() == 1);
    REQUIRE(PKB::stmtTable->getAllIfStmtNums().size() == 0);
}

TEST_CASE("QueryOptimizer optimize groupClauses - 1 group, no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // variable v;
    // Select v such that Parent(_, _)
    Clause c11 = Clause("Parent", {"_", "_"}, {}, 0);
    Query q1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}, {}}, true, true);
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());

    // Select BOOLEAN such that Calls*(_, _) with "x" = "x" and 6 = 9
    Clause c21 = Clause("Calls*", {"_", "_"}, {}, 0);
    Clause c22 = Clause("", {"\"x\"", "\"x\""}, {}, 2);
    Clause c23 = Clause("", {"6", "9"}, {}, 2);
    Query q2 = Query({}, {"BOOLEAN"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({}, {"BOOLEAN"}, {{c21, c22, c23}}, true, true);
    REQUIRE(actual2.getDeclarations() == expected2.getDeclarations());
    REQUIRE(actual2.getToSelect() == expected2.getToSelect());
    REQUIRE_THAT(actual2.getClauses(), Catch::Matchers::UnorderedEquals(expected2.getClauses()));
    REQUIRE(actual2.getIsSyntacticallyValid() == expected2.getIsSyntacticallyValid());
    REQUIRE(actual2.getIsSemanticallyValid() == expected2.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize groupClauses - 1 group, with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // stmt s;
    // Select s such that Follows*(6, s) and Affects(6, s)
    Clause c11 = Clause("Follows*", {"6", "s"}, {"s"}, 1);
    Clause c12 = Clause("Affects", {"6", "s"}, {"s"}, 1);
    Query q1 = Query({{"s", STMT_}}, {"s"}, {{c11, c12}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}}, {"s"}, {{}, {c11, c12}}, true, true);;
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());

    // assign a; variable v; while w;
    // Select w pattern a(v, "x") and w(v, _) with a.stmt# = 8
    Clause c21 = Clause("a", {"v", "\"x\""}, {"a", "v"}, 1);
    Clause c22 = Clause("w", {"v", "_"}, {"w", "v"}, 0);
    Clause c23 = Clause("", {"a.stmt#", "8"}, {"a"}, 1);
    Query q2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{}, {c21, c22, c23}}, true, true);
    REQUIRE(actual2.getDeclarations() == expected2.getDeclarations());
    REQUIRE(actual2.getToSelect() == expected2.getToSelect());
    REQUIRE_THAT(actual2.getClauses(), Catch::Matchers::UnorderedEquals(expected2.getClauses()));
    REQUIRE(actual2.getIsSyntacticallyValid() == expected2.getIsSyntacticallyValid());
    REQUIRE(actual2.getIsSemanticallyValid() == expected2.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize groupClauses - 2 groups, with and without synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // if ifs; prog_line n; variable v;
    // Select n such that Affects(_, _) and Next*(n, 11) with 99 = 99 and ifs.stmt# = n pattern ifs(v, _, _)
    Clause c11 = Clause("Affects", {"_", "_"}, {}, 0);
    Clause c12 = Clause("Next*", {"n", "11"}, {"n"}, 1);
    Clause c13 = Clause("", {"99", "99"}, {}, 2);
    Clause c14 = Clause("", {"ifs.stmt#", "n"}, {"ifs", "n"}, 0);
    Clause c15 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Query q1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                     {"n"}, {{c11, c12, c13, c14, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                            {"n"}, {{c11, c13}, {c12, c14, c15}}, true, true);
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize groupClauses - 2 groups, both with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // assign a; variable v; stmt s; read r; while w;
    // Select <a, r> such that Parent(w, r) with a.stmt# = s.stmt# pattern a(v, _"red"_) such that Next*(r, r) and Modifies(s, v)
    Clause c11 = Clause("Parent", {"w", "r"}, {"w", "r"}, 0);
    Clause c12 = Clause("", {"a.stmt#", "s.stmt#"}, {"a", "s"}, 0);
    Clause c13 = Clause("a", {"v", "_\"red\"_"}, {"a", "v"}, 1);
    Clause c14 = Clause("Next*", {"r", "r"}, {"r"}, 0);
    Clause c15 = Clause("Modifies", {"s", "v"}, {"s", "v"}, 0);
    Query q1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                     {"a", "r"}, {{c11, c12, c13, c14, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                            {"a", "r"}, {{}, {c11, c14}, {c12, c13, c15}}, true, true);
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize groupClauses - multiple groups, 1 group with no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // assign a; if ifs; variable v, v1, v2; constant cc; read r; print pn; call c;
    // Select <ifs, cc, v2>
    // such that Uses(c, v1) and Uses(8, "x") and Uses(a, v2) and Modifies(8, "x") and Next(ifs, r)
    // pattern ifs(v, _, _)
    // with pn.varName = v2.varName and c.stmt# = cc.value and 12 = 12
    Clause c11 = Clause("Uses", {"c", "v1"}, {"c", "v1"}, 0);
    Clause c12 = Clause("Uses", {"8", "\"x\""}, {}, 2);
    Clause c13 = Clause("Uses", {"a", "v2"}, {"a", "v2"}, 0);
    Clause c14 = Clause("Modifies", {"8", "\"x\""}, {}, 2);
    Clause c15 = Clause("Next", {"ifs", "r"}, {"ifs", "r"}, 0);
    Clause c16 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Clause c17 = Clause("", {"pn.varName", "v2.varName"}, {"pn", "v2"}, 0);
    Clause c18 = Clause("", {"c.stmt#", "cc.value"}, {"c", "cc"}, 0);
    Clause c19 = Clause("", {"12", "12"}, {}, 2);
    Query q1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"ifs", "cc", "v2"}, {{c11, c12, c13, c14, c15, c16, c17, c18, c19}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"ifs", "cc", "v2"}, {{c12, c14, c19}, {c15, c16}, {c13, c17}, {c11, c18}}, true, true);
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize groupClauses - multiple groups, all groups with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(false);

    // stmt s; assign a; while w, w1, w2; if ifs, ifs1; variable v, v1; constant cc; print pn; call c;
    // Select <w, p, v1, c>
    // pattern ifs1(v1, _, _) with pn.varName = "rice" and v.varName = "green"
    // with cc.value = s.stmt# such that Parent(w1, pn) and Parent*(w, a) and Next*(a, ifs) and Next*(pn, pn)
    // pattern ifs(v, _, _)
    Clause c11 = Clause("ifs1", {"v1", "_", "_"}, {"ifs1", "v1"}, 0);
    Clause c12 = Clause("", {"pn.varName", "\"rice\""}, {"pn"}, 1);
    Clause c13 = Clause("", {"v.varName", "\"green\""}, {"v"}, 1);
    Clause c14 = Clause("", {"cc.value", "s.stmt#"}, {"cc", "s"}, 0);
    Clause c15 = Clause("Parent", {"w1", "pn"}, {"w1", "pn"}, 0);
    Clause c16 = Clause("Parent*", {"w", "a"}, {"w", "a"}, 0);
    Clause c17 = Clause("Next*", {"a", "ifs"}, {"a", "ifs"}, 0);
    Clause c18 = Clause("Next*", {"pn", "pn"}, {"pn"}, 0);
    Clause c19 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Query q1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"w", "p", "v1", "c"}, {{c11, c12, c13, c14, c15, c16, c17, c18, c19}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"w", "p", "v1", "c"}, {{}, {c14}, {c12, c15, c18}, {}, {c11}, {c13, c16, c17, c19}, {}}, true, true);
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize orderClauses - 1 group, no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // variable v;
    // Select v such that Parent(_, _)
    Clause c11 = Clause("Parent", {"_", "_"}, {}, 0);
    Query q1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}}, true, true);
    REQUIRE(actual1 == expected1);

    // Select BOOLEAN such that Calls*(_, _) with "x" = "x" and 6 = 9
    Clause c21 = Clause("Calls*", {"_", "_"}, {}, 0);
    Clause c22 = Clause("", {"\"x\"", "\"x\""}, {}, 2);
    Clause c23 = Clause("", {"6", "9"}, {}, 2);
    Query q2 = Query({}, {"BOOLEAN"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({}, {"BOOLEAN"}, {{c22, c23, c21}}, true, true);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryOptimizer optimize orderClauses - 1 group, with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // stmt s;
    // Select s such that Follows*(6, s) and Affects(6, s)
    Clause c11 = Clause("Follows*", {"6", "s"}, {"s"}, 1);
    Clause c12 = Clause("Affects", {"6", "s"}, {"s"}, 1);
    Query q1 = Query({{"s", STMT_}}, {"s"}, {{c11, c12}}, true, true);;
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}}, {"s"}, {{c12, c11}}, true, true);;
    REQUIRE(actual1 == expected1);

    // assign a; variable v; while w;
    // Select w pattern a(v, "x") and w(v, _) with a.stmt# = 8
    Clause c21 = Clause("a", {"v", "\"x\""}, {"a", "v"}, 1);
    Clause c22 = Clause("w", {"v", "_"}, {"w", "v"}, 0);
    Clause c23 = Clause("", {"a.stmt#", "8"}, {"a"}, 1);
    Query q2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{c23, c21, c22}}, true, true);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryOptimizer optimize orderClauses - 2 groups, with and without synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // if ifs; prog_line n; variable v;
    // Select n such that Affects(_, _) and Next*(n, 11) with 99 = 99 and ifs.stmt# = n pattern ifs(v, _, _)
    Clause c11 = Clause("Affects", {"_", "_"}, {}, 0);
    Clause c12 = Clause("Next*", {"n", "11"}, {"n"}, 1);
    Clause c13 = Clause("", {"99", "99"}, {}, 2);
    Clause c14 = Clause("", {"ifs.stmt#", "n"}, {"ifs", "n"}, 0);
    Clause c15 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Query q1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                     {"n"}, {{c11, c13}, {c12, c14, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                            {"n"}, {{c13, c11}, {c12, c14, c15}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize orderClauses - 2 groups, both with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // assign a; variable v; stmt s; read r; while w;
    // Select <a, r> such that Parent(w, r) with a.stmt# = s.stmt# pattern a(v, _"red"_) such that Next*(r, r) and Modifies(s, v)
    Clause c11 = Clause("Parent", {"w", "r"}, {"w", "r"}, 0);
    Clause c12 = Clause("", {"a.stmt#", "s.stmt#"}, {"a", "s"}, 0);
    Clause c13 = Clause("a", {"v", "_\"red\"_"}, {"a", "v"}, 1);
    Clause c14 = Clause("Next*", {"r", "r"}, {"r"}, 0);
    Clause c15 = Clause("Modifies", {"s", "v"}, {"s", "v"}, 0);
    Query q1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                     {"a", "r"}, {{c11, c14}, {c12, c13, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                            {"a", "r"}, {{c11, c14}, {c13, c12, c15}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize orderClauses - multiple groups, 1 group with no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // assign a; if ifs; variable v, v1, v2; constant cc; read r; print pn; call c;
    // Select <ifs, cc, v2>
    // such that Uses(c, v1) and Uses(8, "x") and Uses(a, v2) and Modifies(8, "x") and Next(ifs, r)
    // pattern ifs(v, _, _)
    // with pn.varName = v2.varName and c.stmt# = cc.value and 12 = 12
    Clause c11 = Clause("Uses", {"c", "v1"}, {"c", "v1"}, 0);
    Clause c12 = Clause("Uses", {"8", "\"x\""}, {}, 2);
    Clause c13 = Clause("Uses", {"a", "v2"}, {"a", "v2"}, 0);
    Clause c14 = Clause("Modifies", {"8", "\"x\""}, {}, 2);
    Clause c15 = Clause("Next", {"ifs", "r"}, {"ifs", "r"}, 0);
    Clause c16 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Clause c17 = Clause("", {"pn.varName", "v2.varName"}, {"pn", "v2"}, 0);
    Clause c18 = Clause("", {"c.stmt#", "cc.value"}, {"c", "cc"}, 0);
    Clause c19 = Clause("", {"12", "12"}, {}, 2);
    Query q1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"ifs", "cc", "v2"}, {{c12, c14, c19}, {c15, c16}, {c13, c17}, {c11, c18}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"ifs", "cc", "v2"}, {{c19, c12, c14}, {c16, c15}, {c17, c13}, {c18, c11}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize orderClauses - multiple groups, all groups with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(false);
    qo.setIsOrderClauses(true);
    qo.setIsOrderGroups(false);
    setUpQo();

    // stmt s; assign a; while w, w1, w2; if ifs, ifs1; variable v, v1; constant cc; print pn; call c;
    // Select <w, p, v1, c>
    // pattern ifs1(v1, _, _) with pn.varName = "rice" and v.varName = "green"
    // with cc.value = s.stmt# such that Parent(w1, pn) and Parent*(w, a) and Next*(a, ifs) and Next*(pn, pn)
    // pattern ifs(v, _, _)
    Clause c11 = Clause("ifs1", {"v1", "_", "_"}, {"ifs1", "v1"}, 0);
    Clause c12 = Clause("", {"pn.varName", "\"rice\""}, {"pn"}, 1);
    Clause c13 = Clause("", {"v.varName", "\"green\""}, {"v"}, 1);
    Clause c14 = Clause("", {"cc.value", "s.stmt#"}, {"cc", "s"}, 0);
    Clause c15 = Clause("Parent", {"w1", "pn"}, {"w1", "pn"}, 0);
    Clause c16 = Clause("Parent*", {"w", "a"}, {"w", "a"}, 0);
    Clause c17 = Clause("Next*", {"a", "ifs"}, {"a", "ifs"}, 0);
    Clause c18 = Clause("Next*", {"pn", "pn"}, {"pn"}, 0);
    Clause c19 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    Query q1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"w", "p", "v1", "c"}, {{c14}, {c12, c15, c18}, {c11}, {c13, c16, c17, c19}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"w", "p", "v1", "c"}, {{c14}, {c12, c15, c18}, {c11}, {c13, c19, c17, c16}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize orderGroups - 1 group no synonym, 1 group no synonym in select, 1 group synonym in select") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(true);

    // if ifs; variable v1, v2; constant cc; print pn; call c;
    // Select <ifs, cc, v2>
    // such that Uses(c, v1) and Uses(8, "x") and Modifies(8, "x")
    // with pn.varName = v1.varName and ifs.stmt# = cc.value and 12 = 12
    Clause c11 = Clause("Uses", { "c", "v1" }, { "c", "v1" }, 0);
    Clause c12 = Clause("Uses", { "8", "\"x\"" }, {}, 2);
    Clause c13 = Clause("Uses", { "ifs", "v2" }, { "ifs", "v2" }, 0);
    Clause c14 = Clause("Modifies", { "8", "\"x\"" }, {}, 2);
    Clause c17 = Clause("", { "pn.varName", "v1.varName" }, { "pn", "v1" }, 0);
    Clause c18 = Clause("", { "ifs.stmt#", "cc.value" }, { "ifs", "cc" }, 0);
    Clause c19 = Clause("", { "12", "12" }, {}, 2);
    Query q1 = Query({ {"ifs", IF_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
        { "ifs", "cc", "v2" }, { {c19, c12, c14, c17, c13, c18, c11} }, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({ {"ifs", IF_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
        { "ifs", "cc", "v2" }, { {c19, c12, c14}, {c17, c11}, {c13, c18} }, true, true);
    REQUIRE(actual1.getClauses() == expected1.getClauses());
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize orderGroups - 2 groups no synonym in select, 1 group synonym in select") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(true);

    // variable v1, v2; constant cc; print pn; call c; stmt s;
    // Select s.stmt#
    // such that Uses(c, v1) and Modifies(s, "x")
    // with pn.varName = v1.varName and s.stmt# = cc.value and v2.stmt# = 12
    Clause c11 = Clause("Uses", { "c", "v1" }, { "c", "v1" }, 0);
    Clause c14 = Clause("Modifies", { "s", "\"x\"" }, {"s"}, 1);
    Clause c17 = Clause("", { "pn.varName", "v1.varName" }, { "pn", "v1" }, 0);
    Clause c18 = Clause("", { "s.stmt#", "cc.value" }, { "s", "cc" }, 0);
    Clause c19 = Clause("", { "v2.stmt#", "12" }, {"v2"}, 2);
    Query q1 = Query({ {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}, {"s", STMT_} },
        { "s.stmt#" }, { {c19, c14, c17, c18, c11} }, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({ {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}, {"s", STMT_} },
        { "s.stmt#" }, { {c17, c11}, {c19}, {c14, c18} }, true, true);
    REQUIRE(actual1.getClauses().size() == expected1.getClauses().size());
    vector<vector<Clause>> actualNoSynInSelect;
    for (int i = 0; i < 2; i++) {
        actualNoSynInSelect.push_back(actual1.getClauses().at(i));
    }
    vector<vector<Clause>> expectedNoSynInSelect;
    for (int i = 0; i < 2; i++) {
        expectedNoSynInSelect.push_back(expected1.getClauses().at(i));
    }
    REQUIRE_THAT(actualNoSynInSelect, Catch::Matchers::UnorderedEquals(expectedNoSynInSelect));
    REQUIRE(actual1.getClauses().at(2) == expected1.getClauses().at(2));
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize orderGroups - multiple groups no synonym in select, multiple groups synonym in select") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(true);

    // stmt s; assign a; while w, w1, w2; if ifs, ifs1; variable v, v1; constant cc; print pn; call c;
    // Select <w, p, v1, c>
    // pattern ifs1(v1, _, _) with pn.varName = "rice" and v.varName = "green"
    // with cc.value = s.stmt# such that Parent(w1, pn) and Parent*(w, a) and Next*(a, ifs) and Next*(pn, pn) and Uses(c, "x")
    // pattern ifs(v, _, _)
    Clause c11 = Clause("ifs1", { "v1", "_", "_" }, { "ifs1", "v1" }, 0);
    Clause c12 = Clause("", { "pn.varName", "\"rice\"" }, { "pn" }, 1);
    Clause c13 = Clause("", { "v.varName", "\"green\"" }, { "v" }, 1);
    Clause c14 = Clause("", { "cc.value", "s.stmt#" }, { "cc", "s" }, 0);
    Clause c15 = Clause("Parent", { "w1", "pn" }, { "w1", "pn" }, 0);
    Clause c16 = Clause("Parent*", { "w", "a" }, { "w", "a" }, 0);
    Clause c17 = Clause("Next*", { "a", "ifs" }, { "a", "ifs" }, 0);
    Clause c18 = Clause("Next*", { "pn", "pn" }, { "pn" }, 0);
    Clause c19 = Clause("ifs", { "v", "_", "_" }, { "ifs", "v" }, 0);
    Clause c20 = Clause("Uses", { "c", "\"x\"" }, { "c" }, 1);
    Query q1 = Query({ {"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
        { "w", "v1", "c" }, { {c11, c12, c13, c14, c15, c16, c17, c18, c19, c20 } }, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({ {"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
        { "w", "v1", "c" }, { {c14}, {c12, c15, c18}, {c11}, {c13, c16, c17, c19}, {c20} }, true, true);
    REQUIRE(actual1.getClauses().size() == expected1.getClauses().size());
    vector<vector<Clause>> actualNoSynInSelect;
    for (int i = 0; i < 2; i++) {
        actualNoSynInSelect.push_back(actual1.getClauses().at(i));
    }
    vector<vector<Clause>> expectedNoSynInSelect;
    for (int i = 0; i < 2; i++) {
        expectedNoSynInSelect.push_back(expected1.getClauses().at(i));
    }
    REQUIRE_THAT(actualNoSynInSelect, Catch::Matchers::UnorderedEquals(expectedNoSynInSelect));
    vector<vector<Clause>> actualSynInSelect;
    for (int i = 2; i < 5; i++) {
        actualSynInSelect.push_back(actual1.getClauses().at(i));
    }
    vector<vector<Clause>> expectedSynInSelect;
    for (int i = 2; i < 5; i++) {
        expectedSynInSelect.push_back(expected1.getClauses().at(i));
    }
    REQUIRE_THAT(actualSynInSelect, Catch::Matchers::UnorderedEquals(expectedSynInSelect));
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize orderGroups - 1 gorup no synonym, multiple groups no synonym in select, multiple groups synonym in select") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(true);

    // stmt s; assign a; while w, w1, w2; if ifs, ifs1; variable v, v1; constant cc; print pn; call c;
    // Select <w, p, v1, c>
    // pattern ifs1(v1, _, _) with pn.varName = "rice" and v.varName = "green"
    // with cc.value = s.stmt# such that Parent(w1, pn) and Parent*(w, a) and Next*(a, ifs) and Next*(pn, pn) and Uses(c, "x")
    // pattern ifs(v, _, _) with 9 = 9
    Clause c11 = Clause("ifs1", { "v1", "_", "_" }, { "ifs1", "v1" }, 0);
    Clause c12 = Clause("", { "pn.varName", "\"rice\"" }, { "pn" }, 1);
    Clause c13 = Clause("", { "v.varName", "\"green\"" }, { "v" }, 1);
    Clause c14 = Clause("", { "cc.value", "s.stmt#" }, { "cc", "s" }, 0);
    Clause c15 = Clause("Parent", { "w1", "pn" }, { "w1", "pn" }, 0);
    Clause c16 = Clause("Parent*", { "w", "a" }, { "w", "a" }, 0);
    Clause c17 = Clause("Next*", { "a", "ifs" }, { "a", "ifs" }, 0);
    Clause c18 = Clause("Next*", { "pn", "pn" }, { "pn" }, 0);
    Clause c19 = Clause("ifs", { "v", "_", "_" }, { "ifs", "v" }, 0);
    Clause c20 = Clause("Uses", { "c", "\"x\"" }, { "c" }, 1);
    Clause c21 = Clause("", { "9", "9" }, { }, 2);
    Query q1 = Query({ {"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
                     { "w", "v1", "c" }, { {c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21 } }, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({ {"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_} },
                            { "w", "v1", "c" }, { {c21}, {c14}, {c12, c15, c18}, {c11}, {c13, c16, c17, c19}, {c20} }, true, true);
    REQUIRE(actual1.getClauses().size() == expected1.getClauses().size());
    REQUIRE(actual1.getClauses().at(0) == expected1.getClauses().at(0));
    vector<vector<Clause>> actualNoSynInSelect;
    for (int i = 1; i < 3; i++) {
        actualNoSynInSelect.push_back(actual1.getClauses().at(i));
    }
    vector<vector<Clause>> expectedNoSynInSelect;
    for (int i = 1; i < 3; i++) {
        expectedNoSynInSelect.push_back(expected1.getClauses().at(i));
    }
    REQUIRE_THAT(actualNoSynInSelect, Catch::Matchers::UnorderedEquals(expectedNoSynInSelect));
    vector<vector<Clause>> actualSynInSelect;
    for (int i = 3; i < 6; i++) {
        actualSynInSelect.push_back(actual1.getClauses().at(i));
    }
    vector<vector<Clause>> expectedSynInSelect;
    for (int i = 3; i < 6; i++) {
        expectedSynInSelect.push_back(expected1.getClauses().at(i));
    }
    REQUIRE_THAT(actualSynInSelect, Catch::Matchers::UnorderedEquals(expectedSynInSelect));
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}

TEST_CASE("QueryOptimizer optimize orderGroups - select BOOLEAN (i.e. all groups no synonym in select)") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);
    qo.setIsOrderClauses(false);
    qo.setIsOrderGroups(true);

    // variable v1, v2; constant cc; print pn; call c; stmt s;
    // Select BOOLEAN
    // such that Uses(c, v1) and Modifies(s, "x")
    // with pn.varName = v1.varName and s.stmt# = cc.value and v2.stmt# = 12
    Clause c11 = Clause("Uses", { "c", "v1" }, { "c", "v1" }, 0);
    Clause c14 = Clause("Modifies", { "s", "\"x\"" }, { "s" }, 1);
    Clause c17 = Clause("", { "pn.varName", "v1.varName" }, { "pn", "v1" }, 0);
    Clause c18 = Clause("", { "s.stmt#", "cc.value" }, { "s", "cc" }, 0);
    Clause c19 = Clause("", { "v2.stmt#", "12" }, { "v2" }, 1);
    Query q1 = Query({ {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}, {"s", STMT_} },
        { "BOOLEAN" }, { {c19, c14, c17, c18, c11} }, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({ {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}, {"s", STMT_} },
        { "BOOLEAN" }, { {c11, c17}, {c19}, {c14, c18} }, true, true);
    REQUIRE_THAT(actual1.getClauses(), Catch::Matchers::UnorderedEquals(expected1.getClauses()));
    REQUIRE(actual1.getDeclarations() == expected1.getDeclarations());
    REQUIRE(actual1.getToSelect() == expected1.getToSelect());
    REQUIRE(actual1.getIsSyntacticallyValid() == expected1.getIsSyntacticallyValid());
    REQUIRE(actual1.getIsSemanticallyValid() == expected1.getIsSemanticallyValid());
}
