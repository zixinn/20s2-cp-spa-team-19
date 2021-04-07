#include "PKB/Next.h"

#include "catch.hpp"
using namespace std;

//    procedure Second {
//    1.    x = 0;
//    2.    i = 5;
//    3.    while(i != 0) {
//    4.      x = x + 2 * y;
//    5.      call third;
//    6.      i = i - 1; }
//    7.    if (x == 1) then {
//    8.      x = x + 1;
//          } else {
//    9.      z = 1; }
//    10.   z = z + x + i;
//    11.   y = z + 2;
//    12.   x = x * y + z;}

// Next(1, 2), Next(2, 3), Next(3, 4), Next(4, 5), Next(5, 6), Next(6, 3), Next(3, 7),
// Next(7, 8), Next(7, 9), Next(8, 10), Next(9, 10), Next(10, 11), Next(11, 12)

// 75 Next stars
// Next*(1, 2),
// Next*(2, 3), Next*(1, 3)
// Next*(3, 4), Next*(2, 4), Next*(1, 4)
// Next*(4, 5), Next*(3, 5), Next*(2, 5), Next*(1, 5)
// Next*(5, 6), Next*(4, 6), Next*(3, 6), Next*(2, 6), Next*(1, 6)
// Next*(6, 3), Next*(5, 3), Next*(4, 3), Next*(3, 3)
// Next*(6, 4), Next*(5, 4), Next*(4, 4)
// Next*(6, 5), Next*(5, 5)
// Next*(6, 6)
// Next*(3, 7), Next*(6, 7), Next*(5, 7), Next*(4, 7), Next*(2, 7), Next*(1, 7)
// Next*(7, 8), Next*(3, 8), Next*(6, 8), Next*(5, 8), Next*(4, 8), Next*(2, 8), Next*(1, 8)
// Next*(7, 9), Next*(3, 9), Next*(6, 9), Next*(5, 9), Next*(4, 9), Next*(2, 9), Next*(1, 9)
// Next*(8, 10), Next*(7, 10), Next*(3, 10), Next*(6, 10), Next*(5, 10), Next*(4, 10), Next*(2, 10), Next*(1, 10)
// Next*(9, 10),
// Next*(10, 11), Next*(8, 11), Next*(9, 11), Next*(7, 11), Next*(3, 11), Next*(6, 11), Next*(5, 11), Next*(4, 11), Next*(2, 11), Next*(1, 11)
// Next*(11, 12), Next*(10, 12), Next*(8, 12), Next*(9, 12), Next*(7, 12), Next*(3, 12), Next*(6, 12), Next*(5, 12), Next*(4, 12), Next*(2, 12), Next*(1, 12)

Next* setUpNextTest() {
    Next* next = new Next();
    next->storeNext(1,2);
    next->storeNext(2,3);
    next->storeNext(3,4);
    next->storeNext(4,5);
    next->storeNext(5,6);
    next->storeNext(6,3);
    next->storeNext(3,7);
    next->storeNext(7,8);
    next->storeNext(7,9);
    next->storeNext(8,10);
    next->storeNext(9,10);
    next->storeNext(10,11);
    next->storeNext(11,12);
    return next;
}

TEST_CASE("storeNext Test") {
    Next* next = new Next();
    REQUIRE(next->storeNext(1,2));
    REQUIRE(next->storeNext(2,3));
    REQUIRE(next->storeNext(3,4));
    REQUIRE(next->storeNext(4,5));
    REQUIRE(next->storeNext(5,6));
    REQUIRE(next->storeNext(6,3));
    REQUIRE(next->storeNext(3,7));
    REQUIRE(next->storeNext(7,8));
    REQUIRE(next->storeNext(7,9));
    REQUIRE(next->storeNext(8,10));
    REQUIRE(next->storeNext(9,10));
    REQUIRE(next->storeNext(10,11));
    REQUIRE(next->storeNext(11,12));
    REQUIRE_FALSE(next->storeNext(1,2));
}

TEST_CASE("isNext Test") {
    Next* next = setUpNextTest();
    REQUIRE(next->isNext(1,2));
    REQUIRE(next->isNext(2,3));
    REQUIRE(next->isNext(3,4));
    REQUIRE(next->isNext(4,5));
    REQUIRE(next->isNext(5,6));
    REQUIRE(next->isNext(6,3));
    REQUIRE(next->isNext(3,7));
    REQUIRE(next->isNext(7,8));
    REQUIRE(next->isNext(7,9));
    REQUIRE(next->isNext(8,10));
    REQUIRE(next->isNext(9,10));
    REQUIRE(next->isNext(10,11));
    REQUIRE(next->isNext(11,12));
}

TEST_CASE("getNext Test") {
    Next* next = setUpNextTest();
    REQUIRE(next->getNext(1) == unordered_set<ProgLine>({2}));
    REQUIRE(next->getNext(2) == unordered_set<ProgLine>({3}));
    REQUIRE(next->getNext(3) == unordered_set<ProgLine>({4, 7}));
    REQUIRE(next->getNext(4) == unordered_set<ProgLine>({5}));
    REQUIRE(next->getNext(5) == unordered_set<ProgLine>({6}));
    REQUIRE(next->getNext(6) == unordered_set<ProgLine>({3}));
    REQUIRE(next->getNext(7) == unordered_set<ProgLine>({8,9}));
    REQUIRE(next->getNext(8) == unordered_set<ProgLine>({10}));
    REQUIRE(next->getNext(9) == unordered_set<ProgLine>({10}));
    REQUIRE(next->getNext(10) == unordered_set<ProgLine>({11}));
    REQUIRE(next->getNext(11) == unordered_set<ProgLine>({12}));
}

TEST_CASE("getPrevious Test") {
    Next* next = setUpNextTest();
    REQUIRE(next->getPrevious(2) == unordered_set<ProgLine>({1}));
    REQUIRE(next->getPrevious(3) == unordered_set<ProgLine>({2, 6}));
    REQUIRE(next->getPrevious(4) == unordered_set<ProgLine>({3}));
    REQUIRE(next->getPrevious(5) == unordered_set<ProgLine>({4}));
    REQUIRE(next->getPrevious(6) == unordered_set<ProgLine>({5}));
    REQUIRE(next->getPrevious(7) == unordered_set<ProgLine>({3}));
    REQUIRE(next->getPrevious(8) == unordered_set<ProgLine>({7}));
    REQUIRE(next->getPrevious(9) == unordered_set<ProgLine>({7}));
    REQUIRE(next->getPrevious(10) == unordered_set<ProgLine>({8,9}));
    REQUIRE(next->getPrevious(11) == unordered_set<ProgLine>({10}));
    REQUIRE(next->getPrevious(12) == unordered_set<ProgLine>({11}));
}

TEST_CASE("getNextSize Test") {
    Next* next = setUpNextTest();
    REQUIRE(next->getNextSize() == 13);
    next->storeNext(11, 13);
    REQUIRE(next->getNextSize() == 14);
}

TEST_CASE("getAllNext Test") {
    Next* next = setUpNextTest();
    pair<vector<ProgLine>, vector<ProgLine> > result = next->getAllNext();
    vector<ProgLine> n1s = result.first;
    vector<ProgLine> n2s = result.second;
    int num_pairs = n1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == next->getNextSize());
    REQUIRE(n2s.size() == num_pairs);
    // check that each pair at the same index has Calls relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(next->isNext(n1s.at(i), n2s.at(i)));
    }
}

// All tests from now on will test for the logic within populateNextStar()
TEST_CASE("getPreviousStar Test") {
    Next* next = setUpNextTest();
    next->populateNextStar();
    REQUIRE(next->getPreviousStar(2) == unordered_set<ProgLine>({1}));
    REQUIRE(next->getPreviousStar(3) == unordered_set<ProgLine>({1,2,3,4,5,6}));
    REQUIRE(next->getPreviousStar(4) == unordered_set<ProgLine>({1,2,3,4,5,6}));
    REQUIRE(next->getPreviousStar(5) == unordered_set<ProgLine>({1,2,3,4,5,6}));
    REQUIRE(next->getPreviousStar(6) == unordered_set<ProgLine>({1,2,3,4,5,6}));
    REQUIRE(next->getPreviousStar(7) == unordered_set<ProgLine>({1,2,3,4,5,6}));
    REQUIRE(next->getPreviousStar(8) == unordered_set<ProgLine>({1,2,3,4,5,6,7}));
    REQUIRE(next->getPreviousStar(9) == unordered_set<ProgLine>({1,2,3,4,5,6,7}));
    REQUIRE(next->getPreviousStar(10) == unordered_set<ProgLine>({1,2,3,4,5,6,7,8,9}));
    REQUIRE(next->getPreviousStar(11) == unordered_set<ProgLine>({1,2,3,4,5,6,7,8,9,10}));
    REQUIRE(next->getPreviousStar(12) == unordered_set<ProgLine>({1,2,3,4,5,6,7,8,9,10,11}));
}

TEST_CASE("getNextStar Test") {
    Next* next = setUpNextTest();
    next->populateNextStar();
    REQUIRE(next->getNextStar(1) == unordered_set<ProgLine>({2,3,4,5,6,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(2) == unordered_set<ProgLine>({3,4,5,6,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(3) == unordered_set<ProgLine>({3,4,5,6,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(4) == unordered_set<ProgLine>({5,6,3,4,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(5) == unordered_set<ProgLine>({6,3,4,5,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(6) == unordered_set<ProgLine>({3,4,5,6,7,8,9,10,11,12}));
    REQUIRE(next->getNextStar(7) == unordered_set<ProgLine>({8,9,10,11,12}));
    REQUIRE(next->getNextStar(8) == unordered_set<ProgLine>({10,11,12}));
    REQUIRE(next->getNextStar(9) == unordered_set<ProgLine>({10,11,12}));
    REQUIRE(next->getNextStar(10) == unordered_set<ProgLine>({11,12}));
    REQUIRE(next->getNextStar(11) == unordered_set<ProgLine>({12}));
    REQUIRE(next->getNextStar(12).empty());
}

TEST_CASE("isNextStar Test") {
    Next* next = setUpNextTest();
    next->populateNextStar();
    REQUIRE(next->isNextStar(1,2));
    REQUIRE(next->isNextStar(1,3));
    REQUIRE(next->isNextStar(2,5));
    REQUIRE(next->isNextStar(4,3));
    REQUIRE_FALSE(next->isNextStar(5,2));
    REQUIRE(next->isNextStar(5,5));
    REQUIRE(next->isNextStar(5,8));
    REQUIRE(next->isNextStar(5,12));
    REQUIRE_FALSE(next->isNextStar(8,9));
}
TEST_CASE("getNextStarSize Test") {
    Next* next = setUpNextTest();
    next->populateNextStar();
    REQUIRE(next->getNextStarSize() == 75);
}

TEST_CASE("getAllNextStar Test") {
    Next* next = setUpNextTest();
    next->populateNextStar();
    pair<vector<ProgLine>, vector<ProgLine> > result = next->getAllNextStar();
    vector<ProgLine> n1s = result.first;
    vector<ProgLine> n2s = result.second;
    int num_pairs = n1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == next->getNextStarSize());
    REQUIRE(n2s.size() == num_pairs);
    // check that each pair at the same index has Calls relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(next->isNextStar(n1s.at(i), n2s.at(i)));
    }
}
