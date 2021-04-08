#include "Utility.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("trim") {
    // leading whitespace
    REQUIRE(trim(" a") == "a");
    REQUIRE(trim("    b c") == "b c");

    // trailing whitespace
    REQUIRE(trim("de f ") == "de f");
    REQUIRE(trim("ghij    ") == "ghij");

    //leading and trailing whitespace
    REQUIRE(trim(" my STRING ") == "my STRING");
    REQUIRE(trim("    my STRING    ") == "my STRING");

    // other types of whitespace
    REQUIRE(trim(" \n\t my STRING ") == "my STRING");
    REQUIRE(trim("    my STRING\n") == "my STRING");

    // whitespace in STRING
    REQUIRE(trim("    whitespace \nin    STRING\t ") == "whitespace \nin    STRING");
}

TEST_CASE("split") {
    // split by comma
    vector<STRING> actual = split("a, b, c", ",");
    vector<STRING> expected = {"a", "b", "c"};
    REQUIRE(actual == expected);

    actual = split("a", ",");
    expected = {"a"};
    REQUIRE(actual == expected);

    // split by semicolon
    actual = split("assign a; while w; Select w such that Parent* (w, a) pattern a (\"count\", _)", ";");
    expected = {"assign a", "while w", "Select w such that Parent* (w, a) pattern a (\"count\", _)"};
    REQUIRE(actual == expected);

    // split by comma with empty STRING
    actual = split("x, y, , z", ",");
    expected = {"x", "y", "", "z"};
    REQUIRE(actual == expected);

    // split by whitespace
    actual = split("f1    g2 h3", " ");
    expected = {"f1", "", "", "", "g2", "h3"};
    REQUIRE(actual == expected);

    // split empty STRING
    actual = split("", " ");
    expected = { };
    REQUIRE(actual == expected);

    // split with delimiter not found
    actual = split("this is a STRING", ",");
    expected = {"this is a STRING"};
    REQUIRE(actual == expected);

    // split by multiple characters
    actual = split("abc xxdefxxxghij", "xx");
    expected = {"abc", "def", "xghij"};
    REQUIRE(actual == expected);
}

TEST_CASE("checkName") {
    // valid name
    REQUIRE(checkName("name"));
    REQUIRE(checkName("NaMe"));
    REQUIRE(checkName("N4m3"));
    REQUIRE(checkName("a"));

    // invalid name
    REQUIRE_FALSE(checkName("1name"));
    REQUIRE_FALSE(checkName("my_name"));
    REQUIRE_FALSE(checkName("my name"));
    REQUIRE_FALSE(checkName("name?"));
    REQUIRE_FALSE(checkName("n@me"));
}

TEST_CASE("checkInteger") {
    // valid
    REQUIRE(checkInteger("123"));
    REQUIRE(checkInteger("0"));

    // invalid
    REQUIRE_FALSE(checkInteger("098"));
    REQUIRE_FALSE(checkInteger(" 45"));
    REQUIRE_FALSE(checkInteger("76?"));
    REQUIRE_FALSE(checkInteger("abc"));
}
