#include "PKB/VarTable.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("storeVarName Test") {
    VarTable* varTable = new VarTable();
    REQUIRE(varTable->storeVarName("a") == 0);
    REQUIRE(varTable->storeVarName("b") == 1);
    REQUIRE(varTable->getSize() == 2);
    // Adding existing variable
    REQUIRE(varTable->storeVarName("a") == 0);
    REQUIRE(varTable->getSize() == 2);
}

TEST_CASE("getSize Test [VarTable]") {
    VarTable* varTable = new VarTable();
    REQUIRE(varTable->getSize() == 0);
    varTable->storeVarName("a");
    REQUIRE(varTable->getSize() == 1);
}

TEST_CASE("hasVar Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    REQUIRE(varTable->hasVar("a"));
    REQUIRE(varTable->hasVar("b"));
    REQUIRE_FALSE(varTable->hasVar("c"));
}

TEST_CASE("getVarID Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    REQUIRE(varTable->getVarID("a") == 0);
    REQUIRE(varTable->getVarID("b") == 1);
}

TEST_CASE("getVarName Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    REQUIRE(varTable->getVarName(0) == "a");
    REQUIRE(varTable->getVarName(1) == "b");
    REQUIRE_THROWS_AS(varTable->getVarName(2), std::out_of_range);
    REQUIRE_THROWS_AS(varTable->getVarName(-1), std::out_of_range);
}

TEST_CASE("getAllVarNames Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    vector<string> const &varNames = varTable->getAllVarNames();
    REQUIRE_THAT(varNames, Catch::Matchers::UnorderedEquals(vector<string>{ "a", "b"}));
}

TEST_CASE("convertVarIDsToNames Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    varTable->storeVarName("c");
    varTable->storeVarName("d");
    vector<string> varNames = varTable->convertVarIDsToNames(vector<int>{1, 2});
    REQUIRE_THAT(varNames, Catch::Matchers::UnorderedEquals(vector<string>{ "b", "c"}));
}

TEST_CASE("getAllVarIDs Test") {
    VarTable* varTable = new VarTable();
    varTable->storeVarName("a");
    varTable->storeVarName("b");
    varTable->storeVarName("c");
    varTable->storeVarName("d");
    REQUIRE_THAT(varTable->getAllVarIDs(), Catch::Matchers::UnorderedEquals(vector<ID>{ 1,2,3,4}));
}

