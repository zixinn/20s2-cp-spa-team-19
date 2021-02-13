#include "PKB/Parent.h"

#include "catch.hpp"
using namespace std;

Parent* setUpParentTest() {
    Parent* parent = new Parent();
    parent->storeParent(1,2); // 1. while (...) {
    parent->storeParent(2,3); // 2.    while (...) {
    parent->storeParent(3,4); // 3.        while (...) {
    parent->storeParent(2,5); // 4.            b = 2; }
    parent->storeParent(1,6); // 5.        c = 3; }
    parent->storeParent(1,7); // 6.     d = 5;
    return parent;                    // 7.     e = 6; }
}

TEST_CASE("storeParent Test") {
    Parent* parent = new Parent();
    REQUIRE(parent->storeParent(1,2));// 1. while (...) {
    REQUIRE(parent->storeParent(2,3));// 2.    while (...) {
    REQUIRE(parent->storeParent(3,4));// 3.        while (...) {
    REQUIRE(parent->storeParent(2,5));// 4.            b = 2; }
    REQUIRE(parent->storeParent(1,6));// 5.        c = 3; }
    REQUIRE(parent->storeParent(1,7));// 6.     d = 5;
                                      // 7.     e = 6; }
    REQUIRE_FALSE(parent->storeParent(1,3)); // 3 is already child of statement 2
    REQUIRE_FALSE(parent->storeParent(2,4)); // 4 is already child of statement 3
    REQUIRE_FALSE(parent->storeParent(1,2)); // already stored
}

TEST_CASE("populateParentStar Test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->getChildrenStar(1) == unordered_set<StmtNum>({2,3,4,5,6,7}));
    REQUIRE(parent->getChildrenStar(2) == unordered_set<StmtNum>({3,4,5}));
    REQUIRE(parent->getChildrenStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(parent->getChildrenStar(7).empty());
    REQUIRE(parent->getParentStar(1).empty());
    REQUIRE(parent->getParentStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(parent->getParentStar(5) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(6) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(7) == unordered_set<StmtNum>({1}));
}

TEST_CASE("getParentSize Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getParentSize() == 6);
    parent->storeParent(10, 11);
    REQUIRE(parent->getParentSize() == 7);
}

TEST_CASE("getParentStarSize Test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->getParentStarSize() == 10);
}

TEST_CASE("isParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->isParent(1,2));
    REQUIRE(parent->isParent(2,3));
    REQUIRE(parent->isParent(3,4));
    REQUIRE(parent->isParent(2,5));
    REQUIRE(parent->isParent(1,6));
    REQUIRE(parent->isParent(1,7));
    REQUIRE_FALSE(parent->isParent(1,3));
    REQUIRE_FALSE(parent->isParent(1,5));
}

TEST_CASE("isParentStar Test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->isParentStar(1,2));
    REQUIRE(parent->isParentStar(2,3));
    REQUIRE(parent->isParentStar(3,4));
    REQUIRE(parent->isParentStar(2,5));
    REQUIRE(parent->isParentStar(1,6));
    REQUIRE(parent->isParentStar(1,7));
    REQUIRE(parent->isParentStar(1,3));
    REQUIRE(parent->isParentStar(1,4));
    REQUIRE(parent->isParentStar(1,5));
    REQUIRE(parent->isParentStar(2,4));
    REQUIRE_FALSE(parent->isParentStar(2,1));
    REQUIRE_FALSE(parent->isParentStar(3,5));
}

TEST_CASE("hasChild Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->hasChild(1));
    REQUIRE(parent->hasChild(2));
    REQUIRE(parent->hasChild(3));
    REQUIRE_FALSE(parent->hasChild(4));
    REQUIRE_FALSE(parent->hasChild(5));
    REQUIRE_FALSE(parent->hasChild(6));
}

TEST_CASE("hasParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->hasParent(2));
    REQUIRE(parent->hasParent(3));
    REQUIRE(parent->hasParent(4));
    REQUIRE(parent->hasParent(5));
    REQUIRE(parent->hasParent(6));
    REQUIRE(parent->hasParent(7));
    REQUIRE_FALSE(parent->hasParent(1));
}

TEST_CASE("getChildren Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getChildren(1) == unordered_set<StmtNum>({2,6,7}));
    REQUIRE(parent->getChildren(2) == unordered_set<StmtNum>({3,5}));
    REQUIRE(parent->getChildren(3) == unordered_set<StmtNum>({4}));
    REQUIRE(parent->getChildren(4).empty());
}

TEST_CASE("getParent Test") {
    Parent* parent = setUpParentTest();
    REQUIRE(parent->getParent(1) == -1);
    REQUIRE(parent->getParent(2) == 1);
    REQUIRE(parent->getParent(3) == 2);
    REQUIRE(parent->getParent(4) == 3);
    REQUIRE(parent->getParent(5) == 2);
    REQUIRE(parent->getParent(6) == 1);
    REQUIRE(parent->getParent(7) == 1);
}

TEST_CASE("getChildrenStar test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->getChildrenStar(1) == unordered_set<StmtNum>({2,3,4,5,6,7}));
    REQUIRE(parent->getChildrenStar(2) == unordered_set<StmtNum>({3,4,5}));
    REQUIRE(parent->getChildrenStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(parent->getChildrenStar(10).empty());
}

TEST_CASE("getParentStar Test") {
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
    REQUIRE(parent->getParentStar(1).empty());
    REQUIRE(parent->getParentStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(parent->getParentStar(5) == unordered_set<StmtNum>({1,2}));
    REQUIRE(parent->getParentStar(6) == unordered_set<StmtNum>({1}));
    REQUIRE(parent->getParentStar(7) == unordered_set<StmtNum>({1}));
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
    Parent* parent = setUpParentTest();
    parent->populateParentStar();
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

