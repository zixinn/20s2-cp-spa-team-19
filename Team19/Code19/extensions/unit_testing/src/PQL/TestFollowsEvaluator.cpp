#include "PQL/FollowsEvaluator.h"
#include "catch.hpp"

#include <set>

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index): ast::Stmt(new sp::Token(), index){};
};

void setupFollows() {
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
}

TEST_CASE("FollowsEvaluator evaluate known known") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"3", "4"}, {}, 2), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"10", "14"}, {}, 2), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"5", "6"}, {}, 2), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);

    unordered_map<STRING, vector<int>> tempResults4;
    bool b4 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"11", "13"}, {}, 2), tempResults4);
    unordered_map<STRING, vector<int>> expected4 = {};
    REQUIRE_FALSE(b4);
    REQUIRE(tempResults4 == expected4);
}

TEST_CASE("FollowsEvaluator evaluate known synonym") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({{"w", WHILE_}}, Clause("Follows", vector<STRING>{"4", "w"}, {"w"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"w", vector<int>{5}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows", vector<STRING>{"5", "a"}, {"a"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsEvaluator::evaluate({{"s", STMT_}}, Clause("Follows", vector<STRING>{"14", "s"}, {"s"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("FollowsEvaluator evaluate known underscore") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"4", "_"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"14", "_"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsEvaluator evaluate synonym known") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({{"ifs", IF_}}, Clause("Follows", vector<STRING>{"ifs", "14"}, {"ifs"}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {{"ifs", vector<int>{10}}};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({{"c", CALL_}}, Clause("Follows", vector<STRING>{"c", "8"}, {"c"}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);

    unordered_map<STRING, vector<int>> tempResults3;
    bool b3 = FollowsEvaluator::evaluate({{"s", STMT_}}, Clause("Follows", vector<STRING>{"s", "1"}, {"s"}, 1), tempResults3);
    unordered_map<STRING, vector<int>> expected3 = {};
    REQUIRE_FALSE(b3);
    REQUIRE(tempResults3 == expected3);
}

TEST_CASE("FollowsEvaluator evaluate underscore known") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"_", "8"}, {}, 1), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"_", "1"}, {}, 1), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsEvaluator evaluate synonym synonym") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({{"a", ASSIGN_}, {"c", CALL_}}, Clause("Follows", vector<STRING>{"a", "c"}, {"a", "c"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{8, 3}}, {"c", vector<int>{9, 4}}};
    set<pair<int, int>> actual1;
    for (int i = 0; i < tempResults1.begin()->second.size(); i++) {
        pair<int, int> p = make_pair(tempResults1["a"].at(i), tempResults1["c"].at(i));
        actual1.insert(p);
    }
    set<pair<int, int>> expected1 = {{make_pair(3, 4), make_pair(8, 9)}};
    REQUIRE(b1);
    REQUIRE(tempResults1.size() == 2);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({{"a", ASSIGN_}, {"w", WHILE_}}, Clause("Follows", vector<STRING>{"a", "w"}, {"a", "w"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsEvaluator evaluate synonym underscore") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows", vector<STRING>{"a", "_"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{1, 12, 2, 3, 6, 7, 8}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {1, 2, 3, 6, 7, 8, 12};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({{"r", READ_}}, Clause("Follows", vector<STRING>{"r", "_"}, {"r"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsEvaluator evaluate underscore synonym") {
    setupFollows();

    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows", vector<STRING>{"_", "a"}, {"a"}, 0), tempResults1);
//    unordered_map<STRING, vector<int>> expected1 = {{"a", vector<int>{2, 13, 7, 8, 3, 14}}};
    unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
    unordered_set<int> expected1 {2, 3, 7, 8, 13, 14};
    REQUIRE(b1);
    REQUIRE(actual1 == expected1);

    unordered_map<STRING, vector<int>> tempResults2;
    bool b2 = FollowsEvaluator::evaluate({{"p", PRINT_}}, Clause("Follows", vector<STRING>{"_", "p"}, {"p"}, 0), tempResults2);
    unordered_map<STRING, vector<int>> expected2 = {};
    REQUIRE_FALSE(b2);
    REQUIRE(tempResults2 == expected2);
}

TEST_CASE("FollowsEvaluator evaluate underscore underscore") {
    setupFollows();
    unordered_map<STRING, vector<int>> tempResults1;
    bool b1 = FollowsEvaluator::evaluate({}, Clause("Follows", vector<STRING>{"_", "_"}, {}, 0), tempResults1);
    unordered_map<STRING, vector<int>> expected1 = {};
    REQUIRE(b1);
    REQUIRE(tempResults1 == expected1);
}
