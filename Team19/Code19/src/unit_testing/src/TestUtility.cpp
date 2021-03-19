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
    REQUIRE(trim(" my string ") == "my string");
    REQUIRE(trim("    my string    ") == "my string");

    // other types of whitespace
    REQUIRE(trim(" \n\t my string ") == "my string");
    REQUIRE(trim("    my string\n") == "my string");

    // whitespace in string
    REQUIRE(trim("    whitespace \nin    string\t ") == "whitespace \nin    string");
}

TEST_CASE("split") {
    // split by comma
    vector<string> actual = split("a, b, c", ",");
    vector<string> expected = {"a", "b", "c"};
    REQUIRE(actual == expected);

    actual = split("a", ",");
    expected = {"a"};
    REQUIRE(actual == expected);

    // split by semicolon
    actual = split("assign a; while w; Select w such that Parent* (w, a) pattern a (\"count\", _)", ";");
    expected = {"assign a", "while w", "Select w such that Parent* (w, a) pattern a (\"count\", _)"};
    REQUIRE(actual == expected);

    // split by comma with empty string
    actual = split("x, y, , z", ",");
    expected = {"x", "y", "", "z"};
    REQUIRE(actual == expected);

    // split by whitespace
    actual = split("f1    g2 h3", " ");
    expected = {"f1", "", "", "", "g2", "h3"};
    REQUIRE(actual == expected);

    // split empty string
    actual = split("", " ");
    expected = { };
    REQUIRE(actual == expected);

    // split with delimiter not found
    actual = split("this is a string", ",");
    expected = {"this is a string"};
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
