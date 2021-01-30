#include "PQL/QueryPreprocessor.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("process empty query") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "";
    Query actual = qp.process(query);
    Query expected = Query({}, "", {}, {}, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process missing select clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w";
    Query actual = qp.process(query);
    Query expected = Query({}, "", {}, {}, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process multiple select clause") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; while w; \nSelect w such that Parent* (w, a); Select w pattern a (\"count\", _)";
    Query actual = qp.process(query);
    Query expected = Query({}, "", {}, {}, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid design entity in declaration") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign a; While w; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
    Query actual = qp.process(query);
    Query expected = Query({{"a", "assign"}}, "", {}, {}, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process invalid synonym in declaration") {
    QueryPreprocessor qp = QueryPreprocessor();
    string query = "assign 1a; while w; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
    Query actual = qp.process(query);
    Query expected = Query({}, "", {}, {}, false);
    REQUIRE(actual == expected);
}

TEST_CASE("process synonym not declared") {
//    QueryPreprocessor qp = QueryPreprocessor();
//    string query = "assign a; while W; \nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
//    Query actual = qp.process(query);
//    Query expected = Query({{"a", "assign"}, {"W", "while"}}, "", {}, {}, false);
//    REQUIRE(actual == expected);
}

TEST_CASE("process invalid such that clause") {
//    QueryPreprocessor qp = QueryPreprocessor();
//    string query = "assign a; stmt s; \nSelect a such that Uses (a, s)";
//    Query actual = qp.process(query);
//    Query expected = Query({{"a", "assign"}, {"s", "stmt"}}, "a", {}, {}, false);
//    REQUIRE(actual == expected);
//
//    query = "assign a; while w; \nSelect w such that Parent* (w, a) and pattern a (\"count\", _)";
//    actual = qp.process(query);
//    expected = Query({{"a", "assign"}, {"w", "while"}}, "w", {}, {}, false);
//    REQUIRE(actual == expected);
}

TEST_CASE("process invalid pattern clause") {

}

TEST_CASE("process valid query with no such that and pattern clause") {
//    QueryPreprocessor qp = QueryPreprocessor();
//    string query = "variable v; Select v";
//    Query actual = qp.process(query);
//    Query expected = Query({{"v", "variable"}}, "v", {}, {}, true);
//    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with such that clause") {
//    QueryPreprocessor qp = QueryPreprocessor();
//    string query = "assign a; while w; \nSelect w such that Parent* (w, a)";
//    Query actual = qp.process(query);
//    Clause c = Clause("Parent*", {"w", "a"});
//    Query expected = Query({{"a", "assign"}, {"w", "while"}}, "w", {c}, {}, true);
//    REQUIRE(actual == expected);
}

TEST_CASE("process valid query with pattern clause") {

}

TEST_CASE("process valid query with such that and pattern clause") {

}
