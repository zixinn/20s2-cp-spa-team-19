#include "PKB/ConstTable.h"

#include "catch.hpp"
using namespace std;

ConstTable* setupTestTable() {
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
    REQUIRE(constTable->storeConst("1") == true);   // Constant not yet in table
    REQUIRE(constTable->storeConst("1") == false);  // Constant already exists
    REQUIRE(constTable->storeConst("236") == true);   // Constant not yet in table
    REQUIRE(constTable->storeConst(to_string(INT_MAX)) == true);   // Constant not yet in table
    REQUIRE(constTable->getSize() == 3);

    // Invalid Inputs
    REQUIRE_THROWS(constTable->storeConst("acvn"));
    REQUIRE_THROWS(constTable->storeConst("*"));
}

TEST_CASE("getSize Test") {
    ConstTable* constTable = setupTestTable();
    REQUIRE(constTable->getSize() == 5);
    constTable->storeConst("2");
    REQUIRE(constTable->getSize() == 6);
}

TEST_CASE("hasConst Test") {
    ConstTable* constTable = setupTestTable();
    REQUIRE(constTable->hasConst("344") == true);
    REQUIRE(constTable->hasConst("2") == false);
    REQUIRE(constTable->hasConst("heej") == false);

}

TEST_CASE("getConstValue Test") {
    ConstTable* constTable = setupTestTable();
    REQUIRE(constTable->getConstValue("0") == 0);
    REQUIRE(constTable->getConstValue("50") == -1);
    REQUIRE(constTable->getConstValue("asdf") == -1);

}