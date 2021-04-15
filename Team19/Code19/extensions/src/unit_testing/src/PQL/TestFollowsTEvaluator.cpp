#include "PQL/FollowsTEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index): ast::Stmt(new sp::Token(), index){};
};

void setupFollowsT() {
    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);

    PKB::follows = new Follows();
    PKB::follows->storeFollows(1,2);
    PKB::follows->storeFollows(2,3);
    PKB::follows->storeFollows(3,4);
    PKB::follows->storeFollows(4,5);
    PKB::follows->storeFollows(5,10);
    PKB::follows->storeFollows(6,7);
    PKB::follows->storeFollows(7,8);
    PKB::follows->storeFollows(8,9);
    PKB::follows->storeFollows(10,14);
    PKB::follows->storeFollows(12, 13);
    PKB::follows->populateFollowsStar();
}

TEST_CASE("FollowsTEvaluator evaluate known known") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"4", "5"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"3", "10"}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"9", "10"}, {}, 2), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults4;
    bool b4 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"12", "14"}, {}, 2), tempResults4);
    unordered_map<STRING, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("FollowsTEvaluator evaluate known synonym") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({{"w", WHILE_}}, Clause("Follows*", vector<STRING>{"4", "w"}, {"w"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"w", vector<int>{5}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<STRING>{"6", "a"}, {"a"}, 1), tempResults2);
//    unordered_map<STRING, vector<int>> expected2 = {{"a", vector<int>{8, 7}}};
    unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
    unordered_set<int> expected2 {7, 8};
    REQUIRE(b2);
    REQUIRE(actual2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsTEvaluator::evaluate({{"c", CALL_}}, Clause("Follows*", vector<STRING>{"12", "c"}, {"c"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("FollowsTEvaluator evaluate known underscore") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"3", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"11", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsTEvaluator evaluate synonym known") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<STRING>{"a", "14"}, {"a"}, 1), tempResults1);
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {1, 2, 3};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({{"c", CALL_}}, Clause("Follows*", vector<STRING>{"c", "8"}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsTEvaluator::evaluate({{"ifs", IF_}}, Clause("Follows*", vector<STRING>{"ifs", "5"}, {"ifs"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("FollowsTEvaluator evaluate underscore known") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"_", "8"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"_", "11"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsTEvaluator evaluate synonym synonym") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}, {"c", CALL_}}, Clause("Follows*", vector<STRING>{"a", "c"}, {"a", "c"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{1, 2, 3, 6, 7, 8}}, {"c", vector<int>{4, 4, 4, 9, 9, 9}}};
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["a"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(1, 4), make_pair(2, 4), make_pair(3, 4), make_pair(6, 9), make_pair(7, 9), make_pair(8, 9)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({{"ifs", IF_}, {"w", WHILE_}}, Clause("Follows*", vector<STRING>{"ifs", "w"}, {"ifs", "w"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsTEvaluator evaluate synonym underscore") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{8, 7, 6, 3, 2, 12, 1}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {1, 2, 3, 6, 7, 8, 12};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({{"r", READ_}}, Clause("Follows*", vector<STRING>{"r", "_"}, {"r"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsTEvaluator evaluate underscore synonym") {
    setupFollowsT();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<STRING>{"_", "a"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{7, 8, 13, 2, 3, 14}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {2, 3, 7, 8, 13, 14};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsTEvaluator::evaluate({{"p", PRINT_}}, Clause("Follows*", vector<STRING>{"_", "p"}, {"p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsTEvaluator evaluate underscore underscore") {
    setupFollowsT();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<STRING>{"_", "_"}, {}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
