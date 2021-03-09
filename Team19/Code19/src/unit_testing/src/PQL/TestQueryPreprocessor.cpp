#include "PQL/QueryPreprocessor.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("process empty query") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "";
    Query actual = qp.process(query);
    Query expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process select ending with ;") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w; \nSelect w such that Parent* (w, a);";
    Query actual = qp.process(query);
    Query expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process missing select clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w";
    Query actual = qp.process(query);
    Query expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process multiple select clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w; \nSelect w such that Parent* (w, a); Select w pattern a (\"count\", _)";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["w"] = "while";
    Query expected = Query(declarations, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process query not ending with Select clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w; \nSelect w such that Parent* (w, a); variable v";
    Query actual = qp.process(query);
    Query expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid design entity in declaration") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; While w; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    Query expected = Query(declarations, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid synonym in declaration") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign 1a; while w; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
    Query actual = qp.process(query);
    Query expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);

    query = "stmt s, ; \nSelect s";
    actual = qp.process(query);
    expected = Query({}, {}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process synonym not declared") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while W; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["W"] = "while";
    Query expected = Query(declarations, {}, {}, true, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid such that clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; stmt s; \nSelect a such that Uses (a, s)";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["s"] = "stmt";
    Query expected = Query(declarations, {"a"}, {}, true, false);
    REQUIRE(actual == expected);

    query = "assign a; while w; \nSelect a such that Affects* (w, a)";
    actual = qp.process(query);
    unordered_map<string, string> declarations1;
    declarations1["a"] = "assign";
    declarations1["w"] = "while";
    expected = Query(declarations1, {"a"}, {}, true, false);
    REQUIRE(actual == expected);

    query = "assign a; variable v; \nSelect a such that Modifies (_, v)";
    actual = qp.process(query);
    unordered_map<string, string> declarations2;
    declarations2["a"] = "assign";
    declarations2["v"] = "variable";
    expected = Query(declarations2, {"a"}, {}, true, false);
    REQUIRE(actual == expected);

    query = "assign a; while w; \nSelect w such that Parent* (w, a) abc pattern a (\"count\", _)";
    actual = qp.process(query);
    unordered_map<string, string> declarations3;
    declarations3["a"] = "assign";
    declarations3["w"] = "while";
    expected = Query(declarations3, {"w"}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid pattern clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; \nSelect a pattern v (_, \"count + 1\")";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    Query expected = Query(declarations, {"a"}, {}, false, true);
    REQUIRE(actual == expected);

    query = "stmt s; while w; \nSelect w pattern w (\"x\", s)";
    actual = qp.process(query);
    unordered_map<string, string> declarations1;
    declarations1["s"] = "stmt";
    declarations1["w"] = "while";
    expected = Query(declarations1, {"w"}, {}, false, true);
    REQUIRE(actual == expected);

    query = "if ifs; \nSelect ifs pattern ifs (\"count\", _)";
    actual = qp.process(query);
    unordered_map<string, string> declarations2;
    declarations2["ifs"] = "if";
    expected = Query(declarations2, {"ifs"}, {}, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process query with select BOOLEAN") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; \n Select <BOOLEAN, a> such that Affects (a, 11)";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    Query expected = Query(declarations, {}, {}, true, false);
    REQUIRE(actual == expected);

    query = "Select BOOLEAN such that Next* (2, 9)";
    actual = qp.process(query);
    Clause c = Clause("Next*", {"2", "9"});
    expected = Query({}, {"BOOLEAN"}, { c }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process query with select tuple") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "procedure p, q;\n Select <p, q> such that Calls (p, q)";
    Query actual = qp.process(query);
    Clause c = Clause("Calls", {"p", "q"});
    unordered_map<string, string> declarations;
    declarations["p"] = "procedure";
    declarations["q"] = "procedure";
    Query expected = Query(declarations, {"p", "q"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "while w1, w2, w3;\nSelect <w1, w2, w3> such that Parent* (w1, w2) and Parent* (w2, w3)";
    actual = qp.process(query);
    Clause c1 = Clause("Parent*", {"w1", "w2"});
    Clause c2 = Clause("Parent*", {"w2", "w3"});
    unordered_map<string, string> declarations1;
    declarations1["w1"] = "while";
    declarations1["w2"] = "while";
    declarations1["w3"] = "while";
    expected = Query(declarations1, {"w1", "w2", "w3"}, { c1, c2 }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with no such that and pattern clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "variable v; Select v";
    Query actual = qp.process(query);
    unordered_map<string, string> declarations;
    declarations["v"] = "variable";
    Query expected = Query(declarations, {"v"}, {}, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with comma in declaration") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "stmt s1, s2; \nSelect s1 such that Follows(s1, s2)";
    Query actual = qp.process(query);
    Clause c = Clause("Follows", {"s1", "s2"});
    unordered_map<string, string> declarations;
    declarations["s1"] = "stmt";
    declarations["s2"] = "stmt";
    Query expected = Query(declarations, {"s1"}, { c }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with such that clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w; \nSelect w such that Parent* (w, a)";
    Query actual = qp.process(query);
    Clause c = Clause("Parent*", {"w", "a"});
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["w"] = "while";
    Query expected = Query(declarations, {"w"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "variable v; procedure p;\nSelect p such that Modifies (p, \"x\")";
    actual = qp.process(query);
    c = Clause("Modifies", { "p", "\"x\"" });
    unordered_map<string, string> declarations1;
    declarations1["v"] = "variable";
    declarations1["p"] = "procedure";
    expected = Query(declarations1, {"p"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "prog_line n; stmt s; \nSelect s such that Follows* (s, n)";
    actual = qp.process(query);
    c = Clause("Follows*", { "s", "n" });
    unordered_map<string, string> declarations2;
    declarations2["n"] = "prog_line";
    declarations2["s"] = "stmt";
    expected = Query(declarations2, {"s"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "prog_line n; \nSelect n such that Next (5, n)";
    actual = qp.process(query);
    c = Clause("Next", { "5", "n" });
    unordered_map<string, string> declarations3;
    declarations3["n"] = "prog_line";
    expected = Query(declarations3, {"n"}, { c }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with pattern clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; \nSelect a pattern a (_, \"count + 1\")";
    Query actual = qp.process(query);
    Clause c = Clause("a", { "_", "\"count + 1\"" });
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    Query expected = Query(declarations, {"a"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "while w; \nSelect w pattern w (\"x\", _)";
    actual = qp.process(query);
    c = Clause("w", { "\"x\"", "_" });
    unordered_map<string, string> declarations1;
    declarations1["w"] = "while";
    expected = Query(declarations1, {"w"}, { c }, true, true);
    REQUIRE(actual == expected);

    query = "if ifs; \nSelect ifs pattern ifs (\"count\", _, _)";
    actual = qp.process(query);
    c = Clause("ifs", { "\"count\"", "_", "_" });
    unordered_map<string, string> declarations2;
    declarations2["ifs"] = "if";
    expected = Query(declarations2, {"ifs"}, { c }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with such that and pattern clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w;\nSelect a pattern a (\"x\", _) such that Uses (a, \"x\")";
    Query actual = qp.process(query);
    Clause c1 = Clause("Uses", { "a", "\"x\"" });
    Clause c2 = Clause("a", { "\"x\"", "_" });
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["w"] = "while";
    Query expected = Query(declarations, {"a"}, { c2, c1 }, true, true);
    REQUIRE(actual == expected);

    query = "assign a; while w;\nSelect a such that Uses (a, \"x\") pattern a (\"x\", _) ";
    actual = qp.process(query);
    c1 = Clause("Uses", { "a", "\"x\"" });
    c2 = Clause("a", { "\"x\"", "_" });
    unordered_map<string, string> declarations1;
    declarations1["a"] = "assign";
    declarations1["w"] = "while";
    expected = Query(declarations1, {"a"}, { c1, c2 }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with multiple clauses") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w;\nSelect a pattern a (_, \"x + 1\") such that Parent (w, a) such that Uses (a, \"x\") pattern a (\"x\", _) ";
    Query actual = qp.process(query);
    Clause c1 = Clause("a", { "_", "\"x + 1\"" });
    Clause c2 = Clause("Parent", { "w", "a" });
    Clause c3 = Clause("Uses", { "a", "\"x\"" });
    Clause c4 = Clause("a", { "\"x\"", "_" });
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["w"] = "while";
    Query expected = Query(declarations, {"a"}, { c1, c2, c3, c4 }, true, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid query with and in clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w;\nSelect a such that Parent* (w, a) and Modifies (a, \"x\") and such that Next* (1, a)";
    Query actual = qp.process(query);
    Clause c1 = Clause("Parent*", { "w", "a" });
    unordered_map<string, string> declarations;
    declarations["a"] = "assign";
    declarations["w"] = "while";
    Query expected = Query(declarations, {"a"}, { c1 }, false, true);
    REQUIRE(actual == expected);

    query = "assign a; while w;\nSelect a such that Parent* (w, a) and pattern a (\"x\", _) such that Next* (1, a)";
    actual = qp.process(query);
    unordered_map<string, string> declarations1;
    declarations1["a"] = "assign";
    declarations1["w"] = "while";
    expected = Query(declarations1, {"a"}, { }, false, true);
    REQUIRE(actual == expected);

    query = "assign a; while w;\nSelect a such that Parent* (w, a) pattern a (\"x\", _) and Next* (1, a)";
    actual = qp.process(query);
    c1 = Clause("Parent*", { "w", "a" });
    Clause c2 = Clause("a", { "\"x\"", "_" });
    unordered_map<string, string> declarations2;
    declarations2["a"] = "assign";
    declarations2["w"] = "while";
    expected = Query(declarations1, {"a"}, { c1, c2 }, false, true);
    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with and in clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "prog_line n;\nSelect n such that Next* (5, n) and Next* (n, 12)";
    Query actual = qp.process(query);
    Clause c1 = Clause("Next*", { "5", "n" });
    Clause c2 = Clause("Next*", { "n", "12" });
    unordered_map<string, string> declarations;
    declarations["n"] = "prog_line";
    Query expected = Query(declarations, {"n"}, { c1, c2 }, true, true);
    REQUIRE(actual == expected);

    query = "assign a, a1; while w; \nSelect a pattern a (\"x\", _) pattern a1 (\"y\", _\"1\"_) and w (\"count\", _)";
    actual = qp.process(query);
    c1 = Clause("a", { "\"x\"", "_" });
    c2 = Clause("a1", { "\"y\"", "_\"1\"_" });
    Clause c3 = Clause("w", { "\"count\"", "_" });
    unordered_map<string, string> declarations1;
    declarations1["a"] = "assign";
    declarations1["a1"] = "assign";
    declarations1["w"] = "while";
    expected = Query(declarations1, {"a"}, { c1, c2, c3 }, true, true);
    REQUIRE(actual == expected);
}
