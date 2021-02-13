#include "PKB/Follows.h"

#include "catch.hpp"
using namespace std;

Follows* setUpFollowsTest() {
    Follows* follows = new Follows();
    follows->storeFollows(1,2);
    follows->storeFollows(2,3);
    follows->storeFollows(3,4);
    follows->storeFollows(6,7);
    follows->storeFollows(8,9);
    follows->storeFollows(9,10);
    return follows;
}

TEST_CASE("storeFollows Test") {
    Follows* follows = new Follows();
    REQUIRE(follows->storeFollows(1,2));
    REQUIRE(follows->storeFollows(2,3));
    REQUIRE(follows->storeFollows(3,4));
    REQUIRE_FALSE(follows->storeFollows(1,3));
    REQUIRE_FALSE(follows->storeFollows(2,4));
}

TEST_CASE("populateFollowStar Test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    REQUIRE(follows->getFollowerStar(1) == unordered_set<StmtNum>({2,3,4}));
    REQUIRE(follows->getFollowerStar(2) == unordered_set<StmtNum>({3,4}));
    REQUIRE(follows->getFollowerStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(follows->getFollowerStar(6) == unordered_set<StmtNum>({7}));
    REQUIRE(follows->getFollowerStar(8) == unordered_set<StmtNum>({9,10}));
    REQUIRE(follows->getFollowerStar(9) == unordered_set<StmtNum>({10}));
    REQUIRE(follows->getFollowerStar(10).empty());
    REQUIRE(follows->getFolloweeStar(1).empty());
    REQUIRE(follows->getFolloweeStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(follows->getFolloweeStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(follows->getFolloweeStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(follows->getFolloweeStar(7) == unordered_set<StmtNum>({6}));
    REQUIRE(follows->getFolloweeStar(9) == unordered_set<StmtNum>({8}));
    REQUIRE(follows->getFolloweeStar(10) == unordered_set<StmtNum>({8,9}));
}

TEST_CASE("getFollowsSize Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->getFollowsSize() == 6);
    follows->storeFollows(10, 11);
    REQUIRE(follows->getFollowsSize() == 7);
}

TEST_CASE("getFollowsStarSize Test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    REQUIRE(follows->getFollowsStarSize() == 10);
}

TEST_CASE("isFollows Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->isFollows(1,2));
    REQUIRE(follows->isFollows(2,3));
    REQUIRE(follows->isFollows(3,4));
    REQUIRE(follows->isFollows(6,7));
    REQUIRE(follows->isFollows(8,9));
    REQUIRE(follows->isFollows(9,10));
    REQUIRE_FALSE(follows->isFollows(1,5));
}

TEST_CASE("isFollowsStar Test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    REQUIRE(follows->isFollowsStar(1,2));
    REQUIRE(follows->isFollowsStar(2,3));
    REQUIRE(follows->isFollowsStar(3,4));
    REQUIRE(follows->isFollowsStar(1,3));
    REQUIRE(follows->isFollowsStar(1,4));
    REQUIRE(follows->isFollowsStar(2,4));
    REQUIRE(follows->isFollowsStar(6,7));
    REQUIRE(follows->isFollowsStar(8,9));
    REQUIRE(follows->isFollowsStar(9,10));
    REQUIRE(follows->isFollowsStar(8,10));
    REQUIRE_FALSE(follows->isFollowsStar(6,10));
}

TEST_CASE("hasFollower Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->hasFollower(1));
    REQUIRE(follows->hasFollower(2));
    REQUIRE(follows->hasFollower(3));
    REQUIRE(follows->hasFollower(6));
    REQUIRE(follows->hasFollower(8));
    REQUIRE(follows->hasFollower(9));
    REQUIRE_FALSE(follows->hasFollower(5));
}

TEST_CASE("hasFollowee Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->hasFollowee(2));
    REQUIRE(follows->hasFollowee(3));
    REQUIRE(follows->hasFollowee(4));
    REQUIRE(follows->hasFollowee(7));
    REQUIRE(follows->hasFollowee(9));
    REQUIRE(follows->hasFollowee(10));
    REQUIRE_FALSE(follows->hasFollowee(1));
}

TEST_CASE("getFollower Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->getFollower(1) == 2);
    REQUIRE(follows->getFollower(2) == 3);
    REQUIRE(follows->getFollower(3) == 4);
    REQUIRE(follows->getFollower(6) == 7);
    REQUIRE(follows->getFollower(8) == 9);
    REQUIRE(follows->getFollower(9) == 10);
    REQUIRE(follows->getFollower(10) == -1);
}

TEST_CASE("getFollowee Test") {
    Follows* follows = setUpFollowsTest();
    REQUIRE(follows->getFollowee(2) == 1);
    REQUIRE(follows->getFollowee(3) == 2);
    REQUIRE(follows->getFollowee(4) == 3);
    REQUIRE(follows->getFollowee(7) == 6);
    REQUIRE(follows->getFollowee(9) == 8);
    REQUIRE(follows->getFollowee(10) == 9);
    REQUIRE(follows->getFollowee(1) == -1);
}

TEST_CASE("getFollowerStar test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    REQUIRE(follows->getFollowerStar(1) == unordered_set<StmtNum>({2,3,4}));
    REQUIRE(follows->getFollowerStar(2) == unordered_set<StmtNum>({3,4}));
    REQUIRE(follows->getFollowerStar(3) == unordered_set<StmtNum>({4}));
    REQUIRE(follows->getFollowerStar(6) == unordered_set<StmtNum>({7}));
    REQUIRE(follows->getFollowerStar(8) == unordered_set<StmtNum>({9,10}));
    REQUIRE(follows->getFollowerStar(9) == unordered_set<StmtNum>({10}));
    REQUIRE(follows->getFollowerStar(10).empty());
}

TEST_CASE("getFolloweeStar Test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    REQUIRE(follows->getFolloweeStar(1).empty());
    REQUIRE(follows->getFolloweeStar(2) == unordered_set<StmtNum>({1}));
    REQUIRE(follows->getFolloweeStar(3) == unordered_set<StmtNum>({1,2}));
    REQUIRE(follows->getFolloweeStar(4) == unordered_set<StmtNum>({1,2,3}));
    REQUIRE(follows->getFolloweeStar(7) == unordered_set<StmtNum>({6}));
    REQUIRE(follows->getFolloweeStar(9) == unordered_set<StmtNum>({8}));
    REQUIRE(follows->getFolloweeStar(10) == unordered_set<StmtNum>({8,9}));
}

TEST_CASE("getAllFollows Test") {
    Follows* follows = setUpFollowsTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = follows->getAllFollows();
    vector<StmtNum> s1s = result.first;
    vector<StmtNum> s2s = result.second;
    int num_pairs = s1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == follows->getFollowsSize());
    REQUIRE(s2s.size() == num_pairs);
    // check that each pair at the same index has Follows relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(follows->isFollows(s1s.at(i), s2s.at(i)));
    }
}

TEST_CASE("getAllFollowsStar Test") {
    Follows* follows = setUpFollowsTest();
    follows->populateFollowsStar();
    pair<vector<StmtNum>, vector<StmtNum> > result = follows->getAllFollowsStar();
    vector<StmtNum> s1s = result.first;
    vector<StmtNum> s2s = result.second;
    int num_pairs = s1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == follows->getFollowsStarSize());
    REQUIRE(s2s.size() == num_pairs);
    // check that each pair at the same index has FollowsStar relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(follows->isFollowsStar(s1s.at(i), s2s.at(i)));
    }
}
