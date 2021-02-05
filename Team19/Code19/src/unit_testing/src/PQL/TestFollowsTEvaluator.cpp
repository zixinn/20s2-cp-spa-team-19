//#include "PQL/FollowsTEvaluator.h"
//#include "catch.hpp"
//
//using namespace std;
//
//class StmtNodeStub : public ast::Stmt {
//public:
//    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
//};
//
//void setupFollowsT() {
//    PKB::stmtTable = new StmtTable();
//    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
//    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
//    PKB::stmtTable->storeStmt(5, stmtNodeStub, WHILE_);
//    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
//    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
//    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
//    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);
//
//    PKB::follows = new Follows();
//    PKB::follows->storeFollows(1,2);
//    PKB::follows->storeFollows(2,3);
//    PKB::follows->storeFollows(3,4);
//    PKB::follows->storeFollows(4,5);
//    PKB::follows->storeFollows(5,10);
//    PKB::follows->storeFollows(6,7);
//    PKB::follows->storeFollows(7,8);
//    PKB::follows->storeFollows(8,9);
//    PKB::follows->storeFollows(10,14);
//    PKB::follows->storeFollows(12, 13);
//    PKB::follows->populateFollowsStar();
//}
//
//TEST_CASE("FollowsTEvaluator evaluate known known") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"4", "5"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"3", "10"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE(b2);
//    REQUIRE(tempResults2 == expected2);
//
//    unordered_map<string, vector<int>> tempResults3;
//    bool b3 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"9", "10"}), tempResults3);
//    unordered_map<string, vector<int>> expected3 = {};
//    REQUIRE_FALSE(b3);
//    REQUIRE(tempResults3 == expected3);
//
//    unordered_map<string, vector<int>> tempResults4;
//    bool b4 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"12", "14"}), tempResults4);
//    unordered_map<string, vector<int>> expected4 = {};
//    REQUIRE_FALSE(b4);
//    REQUIRE(tempResults4 == expected4);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate known synonym") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({{"w", WHILE_}}, Clause("Follows*", vector<string>{"4", "w"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {{"w", vector<int>{5}}};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<string>{"6", "a"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {{"a", vector<int>{8, 7}}};
//    REQUIRE(b2);
//    REQUIRE(tempResults2 == expected2);
//
//    unordered_map<string, vector<int>> tempResults3;
//    bool b3 = FollowsTEvaluator::evaluate({{"c", CALL_}}, Clause("Follows*", vector<string>{"12", "c"}), tempResults3);
//    unordered_map<string, vector<int>> expected3 = {};
//    REQUIRE_FALSE(b3);
//    REQUIRE(tempResults3 == expected3);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate known underscore") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"3", "_"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"11", "_"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate synonym known") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<string>{"a", "14"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {{"a", vector<int>{1, 2, 3}}};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({{"c", CALL_}}, Clause("Follows*", vector<string>{"c", "8"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//
//    unordered_map<string, vector<int>> tempResults3;
//    bool b3 = FollowsTEvaluator::evaluate({{"ifs", IF_}}, Clause("Follows*", vector<string>{"ifs", "5"}), tempResults3);
//    unordered_map<string, vector<int>> expected3 = {};
//    REQUIRE_FALSE(b3);
//    REQUIRE(tempResults3 == expected3);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate underscore known") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"_", "8"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"_", "11"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate synonym synonym") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}, {"c", CALL_}}, Clause("Follows*", vector<string>{"a", "c"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {{"a", vector<int>{1, 2, 3, 6, 7, 8}}, {"c", vector<int>{4, 4, 4, 9, 9, 9}}};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({{"ifs", IF_}, {"w", WHILE_}}, Clause("Follows*", vector<string>{"ifs", "w"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate synonym underscore") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<string>{"a", "_"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {{"a", vector<int>{8, 7, 6, 3, 2, 12, 1}}};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({{"r", READ_}}, Clause("Follows*", vector<string>{"r", "_"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate underscore synonym") {
//    setupFollowsT();
//
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({{"a", ASSIGN_}}, Clause("Follows*", vector<string>{"_", "a"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {{"a", vector<int>{7, 8, 13, 2, 3, 14}}};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//
//    unordered_map<string, vector<int>> tempResults2;
//    bool b2 = FollowsTEvaluator::evaluate({{"p", PRINT_}}, Clause("Follows*", vector<string>{"_", "p"}), tempResults2);
//    unordered_map<string, vector<int>> expected2 = {};
//    REQUIRE_FALSE(b2);
//    REQUIRE(tempResults2 == expected2);
//}
//
//TEST_CASE("FollowsTEvaluator evaluate underscore underscore") {
//    setupFollowsT();
//    unordered_map<string, vector<int>> tempResults1;
//    bool b1 = FollowsTEvaluator::evaluate({}, Clause("Follows*", vector<string>{"_", "_"}), tempResults1);
//    unordered_map<string, vector<int>> expected1 = {};
//    REQUIRE(b1);
//    REQUIRE(tempResults1 == expected1);
//}
