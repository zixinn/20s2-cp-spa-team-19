#include "PKB/PKB.h"

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

//    procedure Third {
//    13.   z = 5;
//    14.   v = z;
//    15.   print v;}

//    Affects(1, 4), Affects(1, 8), Affects(1,10), Affects(1,12)
//    Affects(2, 6), Affects(2,10)
//    Affects(4, 8), Affects(4, 10), Affects(4,12), Affects(4,4)
//    Affects(6, 10), Affects(6,6)
//    Affects(8, 10), Affects(8, 12)
//    Affects(9, 10)
//    Affects(10, 11), Affects(10, 12)
//    Affects(11, 12)
//    Affects(13, 14)

//    Affects*(1,4), Affects*(1,8), Affects*(1,10), Affects*(1,12), Affects*(1, 11)
//    Affects*(2,6), Affects*(2,10), Affects*(2,11), Affects*(2,12)
//    Affects*(4,8), Affects*(4,10), Affects*(4,12), Affects*(4,4), Affects*(4,11)
//    Affects*(6, 10), Affects*(6,6), Affects*(6,11), Affects*(6, 12)
//    Affects*(8, 10), Affects*(8,12), Affects*(8, 11)
//    Affects*(9, 10), Affects*(9, 11), Affects*(9, 12)
//    Affects*(10, 11), Affects*(10, 12)
//    Affects*(11,12)
//    Affects*(13,14)

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
};

void setUpAffectsTest() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(1);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(2, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(3, stmtNodeStub, "while");
    PKB::stmtTable->storeStmt(4, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(5, stmtNodeStub, "call");
    PKB::stmtTable->storeStmt(6, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(7, stmtNodeStub, "if");
    PKB::stmtTable->storeStmt(8, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(9, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(10, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(11, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(12, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(13, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(14, stmtNodeStub, "assign");
    PKB::stmtTable->storeStmt(15, stmtNodeStub, "print");

    PKB::next->storeNext(1,2);
    PKB::next->storeNext(2,3);
    PKB::next->storeNext(3,4);
    PKB::next->storeNext(4,5);
    PKB::next->storeNext(5,6);
    PKB::next->storeNext(6,3);
    PKB::next->storeNext(3,7);
    PKB::next->storeNext(7,8);
    PKB::next->storeNext(7,9);
    PKB::next->storeNext(8,10);
    PKB::next->storeNext(9,10);
    PKB::next->storeNext(10,11);
    PKB::next->storeNext(11,12);
    PKB::next->storeNext(13,14);
    PKB::next->storeNext(14,15);
    // proc ID: second = 1, third = 2
    PKB::calls->storeCalls(5,1,2);
    PKB::parent->storeParent(3,4);
    PKB::parent->storeParent(3,5);
    PKB::parent->storeParent(3,6);
    PKB::parent->storeParent(7,8);
    PKB::parent->storeParent(7,9);
    // Var ID: x = 1, i = 2, y = 3, z = 4, v = 5
    PKB::uses->storeStmtUses(3,2);
    PKB::uses->storeStmtUses(4,1);
    PKB::uses->storeStmtUses(4,3);
    PKB::uses->storeStmtUses(6,2);
    PKB::uses->storeStmtUses(3,1);
    PKB::uses->storeStmtUses(3,3);
    PKB::uses->storeStmtUses(7,1);
    PKB::uses->storeStmtUses(8,1);
    PKB::uses->storeStmtUses(10,4);
    PKB::uses->storeStmtUses(10,1);
    PKB::uses->storeStmtUses(10,2);
    PKB::uses->storeStmtUses(11,4);
    PKB::uses->storeStmtUses(12,1);
    PKB::uses->storeStmtUses(12,3);
    PKB::uses->storeStmtUses(12,4);
    PKB::uses->storeStmtUses(14,4);
    PKB::uses->storeStmtUses(15,5);

    PKB::uses->storeProcUses(1, 1);
    PKB::uses->storeProcUses(1, 2);
    PKB::uses->storeProcUses(1, 3);
    PKB::uses->storeProcUses(1, 4);
    PKB::uses->storeProcUses(2, 4);
    PKB::uses->storeProcUses(1, 5);

    PKB::modifies->storeStmtModifies(1,1);
    PKB::modifies->storeStmtModifies(2,2);
    PKB::modifies->storeStmtModifies(3,1);
    PKB::modifies->storeStmtModifies(3,2);
    PKB::modifies->storeStmtModifies(4,1);
    PKB::modifies->storeStmtModifies(6,2);
    PKB::modifies->storeStmtModifies(7,1);
    PKB::modifies->storeStmtModifies(7,4);
    PKB::modifies->storeStmtModifies(8,1);
    PKB::modifies->storeStmtModifies(9,4);
    PKB::modifies->storeStmtModifies(10,4);
    PKB::modifies->storeStmtModifies(11,3);
    PKB::modifies->storeStmtModifies(12,1);
    PKB::modifies->storeStmtModifies(13,4);
    PKB::modifies->storeStmtModifies(14,5);

    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(1,3);
    PKB::modifies->storeProcModifies(1,4);
    PKB::modifies->storeProcModifies(2,4);
    PKB::modifies->storeProcModifies(2,5);

    PKB::populatePKB();
}

TEST_CASE("isAffects Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->isAffects(1,4));
    REQUIRE(PKB::affects->isAffects(1,8));
    REQUIRE(PKB::affects->isAffects(1,10));
    REQUIRE(PKB::affects->isAffects(1,12));
    REQUIRE(PKB::affects->isAffects(2,6));
    REQUIRE(PKB::affects->isAffects(2,10));
    REQUIRE(PKB::affects->isAffects(4,8));
    REQUIRE(PKB::affects->isAffects(4,10));
    REQUIRE(PKB::affects->isAffects(4,12));
    REQUIRE(PKB::affects->isAffects(4,4));
    REQUIRE(PKB::affects->isAffects(6,10));
    REQUIRE(PKB::affects->isAffects(6,6));
    REQUIRE(PKB::affects->isAffects(8,10));
    REQUIRE(PKB::affects->isAffects(8,12));
    REQUIRE(PKB::affects->isAffects(9,10));
    REQUIRE(PKB::affects->isAffects(10,11));
    REQUIRE(PKB::affects->isAffects(10,12));
    REQUIRE(PKB::affects->isAffects(11,12));
    REQUIRE(PKB::affects->isAffects(13,14));
    REQUIRE_FALSE(PKB::affects->isAffects(2,3));
    REQUIRE_FALSE(PKB::affects->isAffects(9,6));
    REQUIRE_FALSE(PKB::affects->isAffects(9,11));
}

TEST_CASE("getAffectsSize Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffectsSize() == 19);
}

TEST_CASE("getAffects Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffects(1) == unordered_set<StmtNum>({4,8,10,12}));
    REQUIRE(PKB::affects->getAffects(2) == unordered_set<StmtNum>({6,10}));
    REQUIRE(PKB::affects->getAffects(3).empty());
    REQUIRE(PKB::affects->getAffects(4) == unordered_set<StmtNum>({4,8,10,12}));
    REQUIRE(PKB::affects->getAffects(5).empty());
    REQUIRE(PKB::affects->getAffects(6) == unordered_set<StmtNum>({6,10}));
    REQUIRE(PKB::affects->getAffects(7).empty());
    REQUIRE(PKB::affects->getAffects(8) == unordered_set<StmtNum>({10,12}));
    REQUIRE(PKB::affects->getAffects(9) == unordered_set<StmtNum>({10}));
    REQUIRE(PKB::affects->getAffects(10) == unordered_set<StmtNum>({11,12}));
    REQUIRE(PKB::affects->getAffects(11) == unordered_set<StmtNum>({12}));
    REQUIRE(PKB::affects->getAffects(12).empty());
    REQUIRE(PKB::affects->getAffects(13) == unordered_set<StmtNum>({14}));
    REQUIRE(PKB::affects->getAffects(14).empty());
    REQUIRE(PKB::affects->getAffects(15).empty());
}

TEST_CASE("getAffected Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffected(1).empty());
    REQUIRE(PKB::affects->getAffected(2).empty());
    REQUIRE(PKB::affects->getAffected(3).empty());
    REQUIRE(PKB::affects->getAffected(4) == unordered_set<StmtNum>({1,4}));
    REQUIRE(PKB::affects->getAffected(5).empty());
    REQUIRE(PKB::affects->getAffected(6) == unordered_set<StmtNum>({2,6}));
    REQUIRE(PKB::affects->getAffected(7).empty());
    REQUIRE(PKB::affects->getAffected(8) == unordered_set<StmtNum>({1,4}));
    REQUIRE(PKB::affects->getAffected(9).empty());
    REQUIRE(PKB::affects->getAffected(10) == unordered_set<StmtNum>({1,2,4,6,8,9}));
    REQUIRE(PKB::affects->getAffected(11) == unordered_set<StmtNum>({10}));
    REQUIRE(PKB::affects->getAffected(12) == unordered_set<StmtNum>({1,4,8,10,11}));
    REQUIRE(PKB::affects->getAffected(13).empty());
    REQUIRE(PKB::affects->getAffected(14) == unordered_set<StmtNum>({13}));
}

TEST_CASE("getAllAffects Test") {
    setUpAffectsTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = PKB::affects->getAllAffects();
    vector<StmtNum> a1s = result.first;
    vector<StmtNum> a2s = result.second;
    int num_pairs = a1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == PKB::affects->getAffectsSize());
    REQUIRE(a2s.size() == num_pairs);
    // check that each pair at the same index has ParentStar relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(PKB::affects->isAffects(a1s.at(i), a2s.at(i)));
    }
}

TEST_CASE("isAffectsStar Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->isAffectsStar(1,4));
    REQUIRE(PKB::affects->isAffectsStar(1,8));
    REQUIRE(PKB::affects->isAffectsStar(1,10));
    REQUIRE(PKB::affects->isAffectsStar(1,12));
    REQUIRE(PKB::affects->isAffectsStar(1,11));
    REQUIRE(PKB::affects->isAffectsStar(2,6));
    REQUIRE(PKB::affects->isAffectsStar(2,10));
    REQUIRE(PKB::affects->isAffectsStar(2,11));
    REQUIRE(PKB::affects->isAffectsStar(2,12));
    REQUIRE(PKB::affects->isAffectsStar(4,8));
    REQUIRE(PKB::affects->isAffectsStar(4,10));
    REQUIRE(PKB::affects->isAffectsStar(4,12));
    REQUIRE(PKB::affects->isAffectsStar(4,4));
    REQUIRE(PKB::affects->isAffectsStar(4,11));
    REQUIRE(PKB::affects->isAffectsStar(6,10));
    REQUIRE(PKB::affects->isAffectsStar(6,6));
    REQUIRE(PKB::affects->isAffectsStar(6,11));
    REQUIRE(PKB::affects->isAffectsStar(6,12));
    REQUIRE(PKB::affects->isAffectsStar(8,10));
    REQUIRE(PKB::affects->isAffectsStar(8,11));
    REQUIRE(PKB::affects->isAffectsStar(8,12));
    REQUIRE(PKB::affects->isAffectsStar(9,10));
    REQUIRE(PKB::affects->isAffectsStar(9,11));
    REQUIRE(PKB::affects->isAffectsStar(9,12));
    REQUIRE(PKB::affects->isAffectsStar(10,11));
    REQUIRE(PKB::affects->isAffectsStar(10,12));
    REQUIRE(PKB::affects->isAffectsStar(11,12));
    REQUIRE(PKB::affects->isAffectsStar(13,14));
}

TEST_CASE("getAffectsStarSize Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffectsStarSize() == 28);
}

TEST_CASE("getAffectsStar Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffectsStar(1) == unordered_set<StmtNum>({4,8,10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(2) == unordered_set<StmtNum>({6,10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(3).empty());
    REQUIRE(PKB::affects->getAffectsStar(4) == unordered_set<StmtNum>({4,8,10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(5).empty());
    REQUIRE(PKB::affects->getAffectsStar(6) == unordered_set<StmtNum>({6,10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(7).empty());
    REQUIRE(PKB::affects->getAffectsStar(8) == unordered_set<StmtNum>({10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(9) == unordered_set<StmtNum>({10,11,12}));
    REQUIRE(PKB::affects->getAffectsStar(10) == unordered_set<StmtNum>({11,12}));
    REQUIRE(PKB::affects->getAffectsStar(11) == unordered_set<StmtNum>({12}));
    REQUIRE(PKB::affects->getAffectsStar(12).empty());
    REQUIRE(PKB::affects->getAffectsStar(13) == unordered_set<StmtNum>({14}));
    REQUIRE(PKB::affects->getAffectsStar(14).empty());
    REQUIRE(PKB::affects->getAffectsStar(15).empty());
}


TEST_CASE("getAffectedStar Test") {
    setUpAffectsTest();
    REQUIRE(PKB::affects->getAffectedStar(1).empty());
    REQUIRE(PKB::affects->getAffectedStar(2).empty());
    REQUIRE(PKB::affects->getAffectedStar(3).empty());
    REQUIRE(PKB::affects->getAffectedStar(4) == unordered_set<StmtNum>({1,4}));
    REQUIRE(PKB::affects->getAffectedStar(5).empty());
    REQUIRE(PKB::affects->getAffectedStar(6) == unordered_set<StmtNum>({2,6}));
    REQUIRE(PKB::affects->getAffectedStar(7).empty());
    REQUIRE(PKB::affects->getAffectedStar(8) == unordered_set<StmtNum>({1,4}));
    REQUIRE(PKB::affects->getAffectedStar(9).empty());
    REQUIRE(PKB::affects->getAffectedStar(10) == unordered_set<StmtNum>({1,2,4,6,8,9}));
    REQUIRE(PKB::affects->getAffectedStar(11) == unordered_set<StmtNum>({1,2,4,6,8,9,10}));
    REQUIRE(PKB::affects->getAffectedStar(12) == unordered_set<StmtNum>({1,2,4,6,8,9,10,11}));
    REQUIRE(PKB::affects->getAffectedStar(13).empty());
    REQUIRE(PKB::affects->getAffectedStar(14) == unordered_set<StmtNum>({13}));
}


TEST_CASE("getAllAffectsStar Test") {
    setUpAffectsTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = PKB::affects->getAllAffectsStar();
    vector<StmtNum> a1s = result.first;
    vector<StmtNum> a2s = result.second;
    int num_pairs = a1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == PKB::affects->getAffectsStarSize());
    REQUIRE(a2s.size() == num_pairs);
    // check that each pair at the same index has ParentStar relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(PKB::affects->isAffectsStar(a1s.at(i), a2s.at(i)));
    }
}
