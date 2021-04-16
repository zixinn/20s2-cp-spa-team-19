#include "PKB/ConstTable.h"

#include "catch.hpp"
using namespace std;

ConstTable* setupConstTestTable() {
    ConstTable* constTable = new ConstTable();
    constTable->storeConst("0");
    constTable->storeConst("1");
    constTable->storeConst("5");
    constTable->storeConst("344");
    constTable->storeConst("5643");
    return constTable;
}

TEST_CASE("storeConst Test") {
    ConstTable* constTable = new ConstTable();
    REQUIRE(constTable->storeConst("1"));   // Constant not yet in table
    REQUIRE_FALSE(constTable->storeConst("1"));  // Constant already exists
    REQUIRE(constTable->storeConst("236"));   // Constant not yet in table
    REQUIRE(constTable->storeConst(to_string(INT_MAX)));   // Constant not yet in table
    REQUIRE(constTable->getSize() == 3);

    // Invalid Inputs
    REQUIRE_THROWS(constTable->storeConst("acvn"));
    REQUIRE_THROWS(constTable->storeConst("*"));
}

TEST_CASE("getSize Test [ConstTable]") {
    ConstTable* constTable = setupConstTestTable();
    REQUIRE(constTable->getSize() == 5);
    constTable->storeConst("2");
    REQUIRE(constTable->getSize() == 6);
}

TEST_CASE("hasConst Test") {
    ConstTable* constTable = setupConstTestTable();
    REQUIRE(constTable->hasConst("344"));
    REQUIRE_FALSE(constTable->hasConst("2"));
    REQUIRE_FALSE(constTable->hasConst("heej"));

}

TEST_CASE("getConstValue Test") {
    ConstTable* constTable = setupConstTestTable();
    REQUIRE(constTable->getConstValue("0") == 0);
    REQUIRE(constTable->getConstValue("50") == -1);
    REQUIRE(constTable->getConstValue("asdf") == -1);

}

TEST_CASE("getAllConsts Test") {
    ConstTable* constTable = setupConstTestTable();
    vector<CONST> const &consts = constTable->getAllConsts();
    REQUIRE_THAT(consts, Catch::Matchers::UnorderedEquals(vector<StmtNum>{0,1,5,344,5643}));
}