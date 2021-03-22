#include "PQL/QueryOptimizer.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("QueryOptimizer optimize groupClauses - 1 group, no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // variable v;
    // Select v such that Parent(_, _)
    Clause c11 = Clause("Parent", {"_", "_"}, {});
    Query q1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"v", VARIABLE_}}, {"v"}, {{c11}, {}}, true, true);
    REQUIRE(actual1 == expected1);

    // Select BOOLEAN such that Calls*(_, _) with "x" = "x" and 6 = 9
    Clause c21 = Clause("Calls*", {"_", "_"}, {});
    Clause c22 = Clause("", {"\"x\"", "\"x\""}, {});
    Clause c23 = Clause("", {"6", "9"}, {});
    Query q2 = Query({}, {"BOOLEAN"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({}, {"BOOLEAN"}, {{c21, c22, c23}}, true, true);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryOptimizer optimize groupClauses - 1 group, with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // stmt s;
    // Select s such that Follows*(6, s) and Affects(6, s)
    Clause c11 = Clause("Follows*", {"6", "s"}, {"s"});
    Clause c12 = Clause("Affects", {"6", "s"}, {"s"});
    Query q1 = Query({{"s", STMT_}}, {"s"}, {{c11, c12}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}}, {"s"}, {{}, {c11, c12}}, true, true);;
    REQUIRE(actual1 == expected1);

    // assign a; variable v; while w;
    // Select w pattern a(v, "x") and w(v, _) with a.stmt# = 8
    Clause c21 = Clause("a", {"v", "\"x\""}, {"a", "v"});
    Clause c22 = Clause("w", {"v", "_"}, {"w", "v"});
    Clause c23 = Clause("", {"a.stmt#", "8"}, {"a"});
    Query q2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{c21, c22, c23}}, true, true);
    Query actual2 = qo.optimize(q2);
    Query expected2 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"w", WHILE_}}, {"w"}, {{}, {c21, c22, c23}}, true, true);
    REQUIRE(actual2 == expected2);
}

TEST_CASE("QueryOptimizer optimize groupClauses - 2 groups, with and without synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // if ifs; prog_line n; variable v;
    // Select n such that Affects(_, _) and Next*(n, 11) with 99 = 99 and ifs.stmt# = n pattern ifs(v, _, _)
    Clause c11 = Clause("Affects", {"_", "_"}, {});
    Clause c12 = Clause("Next*", {"n", "11"}, {"n"});
    Clause c13 = Clause("", {"99", "99"}, {});
    Clause c14 = Clause("", {"ifs.stmt#", "n"}, {"ifs", "n"});
    Clause c15 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"});
    Query q1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                     {"n"}, {{c11, c12, c13, c14, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"ifs", IF_}, {"n", PROGLINE_}, {"v", VARIABLE_}},
                            {"n"}, {{c11, c13}, {c12, c14, c15}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize groupClauses - 2 groups, both with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // assign a; variable v; stmt s; read r; while w;
    // Select <a, r> such that Parent(w, r) with a.stmt# = s.stmt# pattern a(v, _"red"_) such that Next*(r, r) and Modifies(s, v)
    Clause c11 = Clause("Parent", {"w", "r"}, {"w", "r"});
    Clause c12 = Clause("", {"a.stmt#", "s.stmt#"}, {"a", "s"});
    Clause c13 = Clause("a", {"v", "_\"red\"_"}, {"a", "v"});
    Clause c14 = Clause("Next*", {"r", "r"}, {"r"});
    Clause c15 = Clause("Modifies", {"s", "v"}, {"s", "v"});
    Query q1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                     {"a", "r"}, {{c11, c12, c13, c14, c15}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"v", VARIABLE_}, {"s", STMT_}, {"r", READ_}, {"w", WHILE_}},
                            {"a", "r"}, {{}, {c11, c14}, {c12, c13, c15}}, true, true);;
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize groupClauses - multiple groups, 1 group with no synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // assign a; if ifs; variable v, v1, v2; constant cc; read r; print pn; call c;
    // Select <ifs, cc, v2>
    // such that Uses(c, v1) and Uses(8, "x") and Uses(a, v2) and Modifies(8, "x") and Next(ifs, r)
    // pattern ifs(v, _, _)
    // with pn.varName = v2.varName and c.stmt# = cc.value and 12 = 12
    Clause c11 = Clause("Uses", {"c", "v1"}, {"c", "v1"});
    Clause c12 = Clause("Uses", {"8", "\"x\""}, {});
    Clause c13 = Clause("Uses", {"a", "v2"}, {"a", "v2"});
    Clause c14 = Clause("Modifies", {"8", "\"x\""}, {});
    Clause c15 = Clause("Next", {"ifs", "r"}, {"ifs", "r"});
    Clause c16 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"});
    Clause c17 = Clause("", {"pn.varName", "v2.varName"}, {"pn", "v2"});
    Clause c18 = Clause("", {"c.stmt#", "cc.value"}, {"c", "cc"});
    Clause c19 = Clause("", {"12", "12"}, {});
    Query q1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"ifs", "cc", "v2"}, {{c11, c12, c13, c14, c15, c16, c17, c18, c19}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"a", ASSIGN_}, {"ifs", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"v2", VARIABLE_}, {"cc", CONSTANT_}, {"r", READ_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"ifs", "cc", "v2"}, {{c12, c14, c19}, {c15, c16}, {c13, c17}, {c11, c18}}, true, true);
    REQUIRE(actual1 == expected1);
}

TEST_CASE("QueryOptimizer optimize groupClauses - multiple groups, all groups with synonym") {
    QueryOptimizer qo = QueryOptimizer();
    qo.setIsGroup(true);

    // stmt s; assign a; while w, w1, w2; if ifs, ifs1; variable v, v1; constant cc; print pn; call c;
    // Select <w, p, v1, c>
    // pattern ifs1(v1, _, _) with pn.varName = "rice" and v.varName = "green"
    // with cc.value = s.stmt# such that Parent(w1, pn) and Parent*(w, a) and Next*(a, ifs) and Next*(pn, pn)
    // pattern ifs(v, _, _)
    Clause c11 = Clause("ifs1", {"v1", "_", "_"}, {"ifs1", "v1"});
    Clause c12 = Clause("", {"pn.varName", "\"rice\""}, {"pn"});
    Clause c13 = Clause("", {"v.varName", "\"green\""}, {"v"});
    Clause c14 = Clause("", {"cc.value", "s.stmt#"}, {"cc", "s"});
    Clause c15 = Clause("Parent", {"w1", "pn"}, {"w1", "pn"});
    Clause c16 = Clause("Parent*", {"w", "a"}, {"w", "a"});
    Clause c17 = Clause("Next*", {"a", "ifs"}, {"a", "ifs"});
    Clause c18 = Clause("Next*", {"pn", "pn"}, {"pn"});
    Clause c19 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"});
    Query q1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                     {"w", "p", "v1", "c"}, {{c11, c12, c13, c14, c15, c16, c17, c18, c19}}, true, true);
    Query actual1 = qo.optimize(q1);
    Query expected1 = Query({{"s", STMT_}, {"a", ASSIGN_}, {"w", WHILE_}, {"w1", WHILE_}, {"w2", WHILE_}, {"ifs", IF_}, {"ifs1", IF_}, {"v", VARIABLE_}, {"v1", VARIABLE_}, {"cc", CONSTANT_}, {"pn", PRINT_}, {"c", CALL_}},
                            {"w", "p", "v1", "c"}, {{}, {c14}, {c12, c15, c18}, {}, {c11}, {c13, c16, c17, c19}, {}}, true, true);
    REQUIRE(actual1 == expected1);
}
