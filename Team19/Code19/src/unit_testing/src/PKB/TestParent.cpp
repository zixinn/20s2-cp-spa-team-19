#include "PKB/Parent.h"

#include "catch.hpp"
using namespace std;

Parent* setUpParentTest() {
    Parent* parent = new Parent();
    parent->storeParent(1,2);
    parent->storeParent(2,3);
    parent->storeParent(3,4);
    parent->storeParent(6,7);
    parent->storeParent(8,9);
    parent->storeParent(9,10);
    return parent;
}

Parent* setUpParentStarTest() {
    Parent* parent = new Parent();
    parent->storeParent(1,2);
    parent->storeParent(2,3);
    parent->storeParent(3,4);
    parent->storeParent(6,7);
    parent->storeParent(8,9);
    parent->storeParent(9,10);
    parent->storeParentStar(1,2);
    parent->storeParentStar(2,3);
    parent->storeParentStar(3,4);
    parent->storeParentStar(1,3);
    parent->storeParentStar(1,4);
    parent->storeParentStar(2,4);
    parent->storeParentStar(6,7);
    parent->storeParentStar(8,9);
    parent->storeParentStar(9,10);
    parent->storeParentStar(8,10);
    return parent;
}

TEST_CASE("storeParent Test") {
    Parent* parent = new Parent();
    REQUIRE(parent->storeParent(1,2));
    REQUIRE(parent->storeParent(2,3));
    REQUIRE(parent->storeParent(3,4));
    REQUIRE_FALSE(parent->storeParent(1,3));
    REQUIRE_FALSE(parent->storeParent(2,4));
}

TEST_CASE("storeParentStar Test") {
    Parent* parent = new Parent();
    REQUIRE(parent->storeParentStar(1,2));
    REQUIRE(parent->storeParentStar(2,3));
    REQUIRE(parent->storeParentStar(3,4));
    REQUIRE(parent->storeParentStar(1,3));
    REQUIRE(parent->storeParentStar(2,4));
    REQUIRE_FALSE(parent->storeParentStar(1,2));
    REQUIRE_FALSE(parent->storeParentStar(1,3));
}

TEST_CASE("getParentSize Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getParentSize() == 6);
    parent->storeParent(10, 11);
    REQUIRE(parent->getParentSize() == 7);
}

TEST_CASE("getParentStarSize Test") {
    Parent* parent = setUpParentStarTest();
    REQUIRE(parent->getParentStarSize() == 10);
}

TEST_CASE("isParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->isParent(1,2));
    REQUIRE(parent->isParent(2,3));
    REQUIRE(parent->isParent(3,4));
    REQUIRE(parent->isParent(6,7));
    REQUIRE(parent->isParent(8,9));
    REQUIRE(parent->isParent(9,10));
    REQUIRE_FALSE(parent->isParent(2,1));
    REQUIRE_FALSE(parent->isParent(1,5));
}

TEST_CASE("isParentStar Test") {
    Parent* parent = setUpParentStarTest();
    REQUIRE(parent->isParentStar(1,2));
    REQUIRE(parent->isParentStar(2,3));
    REQUIRE(parent->isParentStar(3,4));
    REQUIRE(parent->isParentStar(1,3));
    REQUIRE(parent->isParentStar(1,4));
    REQUIRE(parent->isParentStar(2,4));
    REQUIRE(parent->isParentStar(6,7));
    REQUIRE(parent->isParentStar(8,9));
    REQUIRE(parent->isParentStar(9,10));
    REQUIRE(parent->isParentStar(8,10));
    REQUIRE_FALSE(parent->isParentStar(2,1));
    REQUIRE_FALSE(parent->isParentStar(3,1));
    REQUIRE_FALSE(parent->isParentStar(6,10));
}

TEST_CASE("hasChild Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->hasChild(1));
    REQUIRE(parent->hasChild(2));
    REQUIRE(parent->hasChild(3));
    REQUIRE(parent->hasChild(6));
    REQUIRE(parent->hasChild(8));
    REQUIRE(parent->hasChild(9));
    REQUIRE_FALSE(parent->hasChild(5));
}

TEST_CASE("hasParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->hasParent(2));
    REQUIRE(parent->hasParent(3));
    REQUIRE(parent->hasParent(4));
    REQUIRE(parent->hasParent(7));
    REQUIRE(parent->hasParent(9));
    REQUIRE(parent->hasParent(10));
    REQUIRE_FALSE(parent->hasParent(1));
}

TEST_CASE("getChild Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getChild(1) == 2);
    REQUIRE(parent->getChild(2) == 3);
    REQUIRE(parent->getChild(3) == 4);
    REQUIRE(parent->getChild(6) == 7);
    REQUIRE(parent->getChild(8) == 9);
    REQUIRE(parent->getChild(9) == 10);
}

TEST_CASE("getParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getParent(2) == 1);
    REQUIRE(parent->getParent(3) == 2);
    REQUIRE(parent->getParent(4) == 3);
    REQUIRE(parent->getParent(7) == 6);
    REQUIRE(parent->getParent(9) == 8);
    REQUIRE(parent->getParent(10) == 9);
}

TEST_CASE("getChildStar test") {
    Parent* parent = setUpParentStarTest();
    REQUIRE(parent->getChildStar(1) == unordered_set<StmtNum>({2,3,4}));
    REQUIRE(parent->getChildStar(2) == unordered_set<StmtNum>({3,4}));
    REQUIRE(parent->getChildStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(parent->getChildStar(6) == unordered_set<StmtNum>({7}));
    REQUIRE(parent->getChildStar(8) == unordered_set<StmtNum>({9,10}));
    REQUIRE(parent->getChildStar(9) == unordered_set<StmtNum>({10}));
    REQUIRE(parent->getChildStar(10).empty());
}

TEST_CASE("getParentStar Test") {
    Parent* parent = setUpParentStarTest();
    REQUIRE(parent->getParentStar(1).empty());
    REQUIRE(parent->getParentStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(parent->getParentStar(7) == unordered_set<StmtNum>({6}));
    REQUIRE(parent->getParentStar(9) == unordered_set<StmtNum>({8}));
    REQUIRE(parent->getParentStar(10) == unordered_set<StmtNum>({8,9}));
}

TEST_CASE("getAllParent Test") {
    Parent* parent = setUpParentTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = parent->getAllParent();
    vector<StmtNum> s1s = result.first;
    vector<StmtNum> s2s = result.second;
    int num_pairs = s1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == parent->getParentSize());
    REQUIRE(s2s.size() == num_pairs);
    // check that each pair at the same index has Parent relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(parent->isParent(s1s.at(i), s2s.at(i)));
    }
}

TEST_CASE("getAllParentStar Test") {
    Parent* parent = setUpParentStarTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = parent->getAllParentStar();
    vector<StmtNum> s1s = result.first;
    vector<StmtNum> s2s = result.second;
    int num_pairs = s1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == parent->getParentStarSize());
    REQUIRE(s2s.size() == num_pairs);
    // check that each pair at the same index has ParentStar relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(parent->isParentStar(s1s.at(i), s2s.at(i)));
    }
}

TEST_CASE("populateParentStar Test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->getChildStar(1) == unordered_set<StmtNum>({2,3,4}));
    REQUIRE(parent->getChildStar(2) == unordered_set<StmtNum>({3,4}));
    REQUIRE(parent->getChildStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(parent->getChildStar(6) == unordered_set<StmtNum>({7}));
    REQUIRE(parent->getChildStar(8) == unordered_set<StmtNum>({9,10}));
    REQUIRE(parent->getChildStar(9) == unordered_set<StmtNum>({10}));
    REQUIRE(parent->getParentStar(1).empty());
    REQUIRE(parent->getParentStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(parent->getParentStar(7) == unordered_set<StmtNum>({6}));
    REQUIRE(parent->getParentStar(9) == unordered_set<StmtNum>({8}));
    REQUIRE(parent->getParentStar(10) == unordered_set<StmtNum>({8,9}));
}
