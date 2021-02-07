#include "SP/DesignExtractor.h"
#include "PKB/PKB.h"

#include "catch.hpp"
using namespace std;

// Set up PKB
void setupPKBAndCleamDE() {
    DesignExtractor::signalReset(); // Resets both DE and PKB
    PKB::constTable->storeConst("6");
}

TEST_CASE("storeNewProcedureTest") {

}

// Test cases for stacks and related methods
TEST_CASE("Generic Stack Operations Test") {
    vector<vector<int>> stmtLstsStack;
    vector<set<ID>> usesStack;
    vector<set<ID>> modifiesStack;
    vector<int> parentStack;

    // Test stmtLstsStack
    vector<ID> numbers{ 0, 1, 3, 400, 70000};
    // -3 is an invalid stmtNum, but DesignExtractor assumes all inputs from Parser are valid.
    vector<ID> numbers2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(stmtLstsStack.size() == 0);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    REQUIRE(stmtLstsStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == numbers);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == vector<ID>{});
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers2);
    REQUIRE(stmtLstsStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == numbers2);

    // Test usesStack
    set<ID> usesIDs{ 0, 1, 3, 400, 70000};
    set<ID> usesIDs2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(usesStack.size() == 0);
    DesignExtractor::DEStack<set<ID>>::stackPush(usesStack, usesIDs);
    REQUIRE(usesStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == usesIDs);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == set<ID>{});
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs2);
    REQUIRE(usesStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == usesIDs2);

    // Test modifiesStack
    set<ID> modifiesIDs{ 0, 1, 3, 400, 70000};
    set<ID> modifiesIDs2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(modifiesStack.size() == 0);
    DesignExtractor::DEStack<set<ID>>::stackPush(modifiesStack, usesIDs);
    REQUIRE(modifiesStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == usesIDs);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == set<ID>{});
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs2);
    REQUIRE(modifiesStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == usesIDs2);

    // Test parentStack
    REQUIRE(parentStack.size() == 0);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 3);
    REQUIRE(parentStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 3);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 1);  // empty parent stack returns 1
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 5);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 356);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 909090);
    REQUIRE(parentStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 909090);
};

