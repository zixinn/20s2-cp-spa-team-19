#include "PKB/ProcTable.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("storeProcName Test") {
    ProcTable* procTable = new ProcTable();
    REQUIRE(procTable->storeProcName("a") == 0);
    REQUIRE(procTable->storeProcName("b") == 1);
    REQUIRE(procTable->getSize() == 2);
    // Adding existing procedure
    REQUIRE(procTable->storeProcName("a") == 0);
    REQUIRE(procTable->getSize() == 2);
}

TEST_CASE("storeProcStmt Test") {
    ProcTable* procTable = new ProcTable();
    REQUIRE(procTable->storeProcStmt(0, 1, 5));
    REQUIRE(procTable->storeProcStmt(1, 6, 10));
    // Storing existing key
    REQUIRE_FALSE(procTable->storeProcStmt(0, 2, 3));
}

TEST_CASE("getProcRange Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcStmt(0, 1, 5);
    procTable->storeProcStmt(1, 6, 10);
    // Storing existing key
    procTable->storeProcStmt(0, 2, 3);

    pair<int, int> range = procTable->getProcRange(0);
    REQUIRE(range.first == 1);
    REQUIRE(range.second == 5);

    pair<int, int> range2 = procTable->getProcRange(1);
    REQUIRE(range2.first == 6);
    REQUIRE(range2.second == 10);

    REQUIRE_THROWS_AS(procTable->getProcRange(2), std::exception);
    REQUIRE_THROWS_AS(procTable->getProcRange(-1), std::exception);
}

TEST_CASE("getSize Test [ProcTable]") {
    ProcTable* procTable = new ProcTable();
    REQUIRE(procTable->getSize() == 0);
    procTable->storeProcName("a");
    REQUIRE(procTable->getSize() == 1);
}

TEST_CASE("hasProc Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcName("a");
    procTable->storeProcName("b");
    REQUIRE(procTable->hasProc("a"));
    REQUIRE(procTable->hasProc("b"));
    REQUIRE_FALSE(procTable->hasProc("c"));
}

TEST_CASE("getProcID Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcName("a");
    procTable->storeProcName("b");
    REQUIRE(procTable->getProcID("a") == 0);
    REQUIRE(procTable->getProcID("b") == 1);
    REQUIRE(procTable->getProcID("c") == -1);
}

TEST_CASE("getProcName Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcName("a");
    procTable->storeProcName("b");
    REQUIRE(procTable->getProcName(0) == "a");
    REQUIRE(procTable->getProcName(1) == "b");
    REQUIRE_THROWS_AS(procTable->getProcName(2), std::out_of_range);
    REQUIRE_THROWS_AS(procTable->getProcName(-1), std::out_of_range);
}

TEST_CASE("getAllProcNames Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcName("a");
    procTable->storeProcName("b");
    vector<string> const &procNames = procTable->getAllProcNames();
    REQUIRE_THAT(procNames, Catch::Matchers::UnorderedEquals(vector<string>{ "a", "b"}));
}

TEST_CASE("convertProcIDsToNames Test") {
    ProcTable* procTable = new ProcTable();
    procTable->storeProcName("a");
    procTable->storeProcName("b");
    procTable->storeProcName("c");
    procTable->storeProcName("d");
    vector<string> procNames = procTable->convertProcIDsToNames(vector<int>{1, 2});
    REQUIRE_THAT(procNames, Catch::Matchers::UnorderedEquals(vector<string>{ "b", "c"}));
}
