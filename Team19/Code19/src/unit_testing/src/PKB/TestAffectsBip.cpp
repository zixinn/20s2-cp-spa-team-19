#include "PKB/PKB.h"
#include "PKB/AffectsBip.h"

#include "catch.hpp"
using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

//    procedure Bill {
//    01      x = 5;
//    02      call Mary;
//    03      y = x + 6;
//    04      call John;
//    05      z = x * y + 2; }
//
//    procedure Mary {
//    06      y = x * 3;
//    07      call John; }
//
//    procedure John {
//    08      if (i > 0) then {
//    09          x = x + z;
//            } else {
//    10          if (j > 0) then {
//    11              x = x + z;
//                } else {
//    12              y = x * y; } }

void setUpAffectsBipTest() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("Bill"); // 0
    PKB::procTable->storeProcName("Mary"); // 1
    PKB::procTable->storeProcName("John"); // 2
    PKB::procTable->storeProcStmt(0, 1, 5);
    PKB::procTable->storeProcStmt(1, 6, 7);
    PKB::procTable->storeProcStmt(2, 8, 8);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(8, 9, set<StmtNum>{9});
    PKB::stmtTable->storeElseStmt(8, 10, set<StmtNum>{11, 12});
    PKB::stmtTable->storeIfStmt(10, 11, set<StmtNum>{11});
    PKB::stmtTable->storeElseStmt(10, 12, set<StmtNum>{12});

    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(4, 0, 2);
    PKB::calls->storeCalls(7, 1, 2);

    PKB::parent->storeParent(8,9);
    PKB::parent->storeParent(8,10);
    PKB::parent->storeParent(10,11);
    PKB::parent->storeParent(10,12);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(8, 10);
    PKB::next->storeNext(10, 11);
    PKB::next->storeNext(10, 12);

    // x: 0, y:1, z:2, i:3, j:4
    PKB::uses->storeStmtUses(3,0);
    PKB::uses->storeStmtUses(5,0);
    PKB::uses->storeStmtUses(5,1);
    PKB::uses->storeStmtUses(6,0);
    PKB::uses->storeStmtUses(8,0);
    PKB::uses->storeStmtUses(8,1);
    PKB::uses->storeStmtUses(8,2);
    PKB::uses->storeStmtUses(8,3);
    PKB::uses->storeStmtUses(8,4);
    PKB::uses->storeStmtUses(9,0);
    PKB::uses->storeStmtUses(9,2);
    PKB::uses->storeStmtUses(10,0);
    PKB::uses->storeStmtUses(10,1);
    PKB::uses->storeStmtUses(10,2);
    PKB::uses->storeStmtUses(10,4);
    PKB::uses->storeStmtUses(11,0);
    PKB::uses->storeStmtUses(11,2);
    PKB::uses->storeStmtUses(12,0);
    PKB::uses->storeStmtUses(12,1);

    PKB::uses->storeProcUses(0,0);
    PKB::uses->storeProcUses(0,1);
    PKB::uses->storeProcUses(1,0);
    PKB::uses->storeProcUses(2,0);
    PKB::uses->storeProcUses(2,1);
    PKB::uses->storeProcUses(2,2);
    PKB::uses->storeProcUses(2,3);
    PKB::uses->storeProcUses(2,4);

    PKB::modifies->storeStmtModifies(1,0);
    PKB::modifies->storeStmtModifies(3,1);
    PKB::modifies->storeStmtModifies(5,2);
    PKB::modifies->storeStmtModifies(6,1);
    PKB::modifies->storeStmtModifies(8,0);
    PKB::modifies->storeStmtModifies(8,1);
    PKB::modifies->storeStmtModifies(9,0);
    PKB::modifies->storeStmtModifies(10,0);
    PKB::modifies->storeStmtModifies(10,1);
    PKB::modifies->storeStmtModifies(11,0);
    PKB::modifies->storeStmtModifies(12,1);

    PKB::modifies->storeProcModifies(0,0);
    PKB::modifies->storeProcModifies(0,1);
    PKB::modifies->storeProcModifies(0,2);
    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(2,0);
    PKB::modifies->storeProcModifies(2,1);

    PKB::populatePKB();
}

TEST_CASE("getAffectsBip Test") {
    setUpAffectsBipTest();
    REQUIRE(PKB::affectsBip->getAffectsBip(1) == unordered_set<ProgLine>{6,9,11,12,3,5});
    REQUIRE(PKB::affectsBip->getAffectsBip(2) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(3) == unordered_set<ProgLine>{12,5});
    REQUIRE(PKB::affectsBip->getAffectsBip(4) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(6) == unordered_set<ProgLine>{12});
    REQUIRE(PKB::affectsBip->getAffectsBip(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(8) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(9) == unordered_set<ProgLine>{3,5,9,11,12});
    REQUIRE(PKB::affectsBip->getAffectsBip(10) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(11) == unordered_set<ProgLine>{3,5,9,11,12});
    REQUIRE(PKB::affectsBip->getAffectsBip(12) == unordered_set<ProgLine>{5});
}

TEST_CASE("getAffectedBip Test") {
    setUpAffectsBipTest();
    REQUIRE(PKB::affectsBip->getAffectedBip(1) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(2) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(3) == unordered_set<ProgLine>{1,9,11});
    REQUIRE(PKB::affectsBip->getAffectedBip(4) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(5) == unordered_set<ProgLine>{1,3,9,11,12});
    REQUIRE(PKB::affectsBip->getAffectedBip(6) == unordered_set<ProgLine>{1});
    REQUIRE(PKB::affectsBip->getAffectedBip(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(8) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(9) == unordered_set<ProgLine>{1,9,11});
    REQUIRE(PKB::affectsBip->getAffectedBip(10) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectedBip(11) == unordered_set<ProgLine>{1,9,11});
    REQUIRE(PKB::affectsBip->getAffectedBip(12) == unordered_set<ProgLine>{1,3,6,9,11});
}

TEST_CASE("isAffectsBip Test") {
    setUpAffectsBipTest();
    REQUIRE(PKB::affectsBip->isAffectsBip(1,6));
    REQUIRE(PKB::affectsBip->isAffectsBip(1,9));
    REQUIRE(PKB::affectsBip->isAffectsBip(1,11));
    REQUIRE(PKB::affectsBip->isAffectsBip(1,12));
    REQUIRE(PKB::affectsBip->isAffectsBip(1,3));
    REQUIRE(PKB::affectsBip->isAffectsBip(1,5));
    REQUIRE(PKB::affectsBip->isAffectsBip(3,12));
    REQUIRE(PKB::affectsBip->isAffectsBip(3,5));
    REQUIRE(PKB::affectsBip->isAffectsBip(6,12));
    REQUIRE(PKB::affectsBip->isAffectsBip(9,3));
    REQUIRE(PKB::affectsBip->isAffectsBip(9,5));
    REQUIRE(PKB::affectsBip->isAffectsBip(9,9));
    REQUIRE(PKB::affectsBip->isAffectsBip(9,11));
    REQUIRE(PKB::affectsBip->isAffectsBip(9,12));
    REQUIRE(PKB::affectsBip->isAffectsBip(11,3));
    REQUIRE(PKB::affectsBip->isAffectsBip(11,5));
    REQUIRE(PKB::affectsBip->isAffectsBip(11,9));
    REQUIRE(PKB::affectsBip->isAffectsBip(11,11));
    REQUIRE(PKB::affectsBip->isAffectsBip(11,12));
    REQUIRE(PKB::affectsBip->isAffectsBip(12,5));
}

TEST_CASE("getAffectsBipSize Test") {
    setUpAffectsBipTest();
    REQUIRE(PKB::affectsBip->getAffectsBipSize() == 20);
}

TEST_CASE("getAllAffectsBip Test") {
    setUpAffectsBipTest();
    pair<vector<StmtNum>, vector<StmtNum> > result = PKB::affectsBip->getAllAffectsBip();
    vector<StmtNum> a1s = result.first;
    vector<StmtNum> a2s = result.second;
    int num_pairs = a1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == PKB::affectsBip->getAffectsBipSize());
    REQUIRE(a2s.size() == num_pairs);
    // check that each pair at the same index has ParentStar relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(PKB::affectsBip->isAffectsBip(a1s.at(i), a2s.at(i)));
    }
}

//    procedure Bill {
//        01      x = 5;
//        02      call Mary;
//        03      y = x + 6;
//        04      call John;
//        05      z = x * y + 2; }
//
//    procedure Mary {
//        06      y = x * 3;
//        07      call John;
//        08      z = x + y; }
//
//    procedure John {
//        09      if (i > 0) then {
//        10              x = x + z; } }
//                else {
//        11              y = x * y; } }

void setUpAffectsBipTest2() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("Bill"); // 0
    PKB::procTable->storeProcName("Mary"); // 1
    PKB::procTable->storeProcName("John"); // 2
    PKB::procTable->storeProcStmt(0, 1, 5);
    PKB::procTable->storeProcStmt(1, 6, 8);
    PKB::procTable->storeProcStmt(2, 9, 9);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(9, 10, set<StmtNum>{10});
    PKB::stmtTable->storeElseStmt(9, 11, set<StmtNum>{11});

    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(4, 0, 2);
    PKB::calls->storeCalls(7, 1, 2);

    PKB::parent->storeParent(9,10);
    PKB::parent->storeParent(9,11);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(7, 8);
    PKB::next->storeNext(9, 10);
    PKB::next->storeNext(9, 11);

    // x: 0, y:1, z:2, i:3
    PKB::uses->storeStmtUses(3,0);
    PKB::uses->storeStmtUses(5,0);
    PKB::uses->storeStmtUses(5,1);
    PKB::uses->storeStmtUses(6,0);
    PKB::uses->storeStmtUses(8,0);
    PKB::uses->storeStmtUses(8,1);
    PKB::uses->storeStmtUses(9,0);
    PKB::uses->storeStmtUses(9,1);
    PKB::uses->storeStmtUses(9,2);
    PKB::uses->storeStmtUses(9,3);
    PKB::uses->storeStmtUses(10,0);
    PKB::uses->storeStmtUses(10,2);
    PKB::uses->storeStmtUses(11,0);
    PKB::uses->storeStmtUses(11,1);

    PKB::uses->storeProcUses(0,0);
    PKB::uses->storeProcUses(0,1);
    PKB::uses->storeProcUses(1,0);
    PKB::uses->storeProcUses(1,1);
    PKB::uses->storeProcUses(2,0);
    PKB::uses->storeProcUses(2,1);
    PKB::uses->storeProcUses(2,2);
    PKB::uses->storeProcUses(2,3);

    PKB::modifies->storeStmtModifies(1,0);
    PKB::modifies->storeStmtModifies(3,1);
    PKB::modifies->storeStmtModifies(5,2);
    PKB::modifies->storeStmtModifies(6,1);
    PKB::modifies->storeStmtModifies(8,2);
    PKB::modifies->storeStmtModifies(9,0);
    PKB::modifies->storeStmtModifies(9,1);
    PKB::modifies->storeStmtModifies(10,0);
    PKB::modifies->storeStmtModifies(11,1);

    PKB::modifies->storeProcModifies(0,0);
    PKB::modifies->storeProcModifies(0,1);
    PKB::modifies->storeProcModifies(0,2);
    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(2,0);
    PKB::modifies->storeProcModifies(2,1);

    PKB::populatePKB();
}

TEST_CASE("getAffectsBip Test2") {
    setUpAffectsBipTest2();
    REQUIRE(PKB::affectsBip->getAffectsBip(1) == unordered_set<ProgLine>{6,10,11,8,3,5});
    REQUIRE(PKB::affectsBip->getAffectsBip(2) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(3) == unordered_set<ProgLine>{11,5});
    REQUIRE(PKB::affectsBip->getAffectsBip(4) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(6) == unordered_set<ProgLine>{11,8});
    REQUIRE(PKB::affectsBip->getAffectsBip(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(8) == unordered_set<ProgLine>{10});
    REQUIRE(PKB::affectsBip->getAffectsBip(9) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(10) == unordered_set<ProgLine>{8,3,5,10,11});
    REQUIRE(PKB::affectsBip->getAffectsBip(11) == unordered_set<ProgLine>{8,5});
}

TEST_CASE("getAffectsBipSize Test2") {
    setUpAffectsBipTest2();
    REQUIRE(PKB::affectsBip->getAffectsBipSize() == 18);
}

//    procedure a {
//    01    while (x == 1) {
//    02        y = 2;
//    03        while (y == 2) {
//    04            z = x + y
//    05            if (z == 3) then {
//    06                x = z + y;
//    07                call b;
//                  } else {
//    08                call c;
//    09                x = x + 1; } } } }
//
//    procedure b {
//    10    x = y + 1;
//    11    while (y < 5) {
//    12        if (y == 1) then {
//    13            z = z + 1;
//    14            call c;
//    15            x = x + 1;
//              } else {
//    16            z = z + 2; } } }
//
//    procedure c {
//    17    x = 2 + y;
//    }

void setUpAffectsBipTest3() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("a"); // 0
    PKB::procTable->storeProcName("b"); // 1
    PKB::procTable->storeProcName("c"); // 2
    PKB::procTable->storeProcStmt(0, 1, 1);
    PKB::procTable->storeProcStmt(1, 10, 11);
    PKB::procTable->storeProcStmt(2, 17, 17);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(15, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(16, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(17, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(5, 6, set<StmtNum>{7});
    PKB::stmtTable->storeElseStmt(5, 8, set<StmtNum>{9});
    PKB::stmtTable->storeIfStmt(12, 13, set<StmtNum>{15});
    PKB::stmtTable->storeElseStmt(12, 16, set<StmtNum>{16});

    PKB::calls->storeCalls(7, 0, 1);
    PKB::calls->storeCalls(8, 0, 2);
    PKB::calls->storeCalls(14, 1, 2);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(5, 8);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(7, 3);
    PKB::next->storeNext(9, 3);
    PKB::next->storeNext(3, 1);
    PKB::next->storeNext(10, 11);
    PKB::next->storeNext(11, 12);
    PKB::next->storeNext(12, 13);
    PKB::next->storeNext(13, 14);
    PKB::next->storeNext(14, 15);
    PKB::next->storeNext(12, 16);
    PKB::next->storeNext(15, 11);
    PKB::next->storeNext(16, 11);

    // x: 0, y:1, z:2
    PKB::uses->storeStmtUses(1,0);
    PKB::uses->storeStmtUses(1,1);
    PKB::uses->storeStmtUses(1,2);
    PKB::uses->storeStmtUses(3,1);
    PKB::uses->storeStmtUses(3,1);
    PKB::uses->storeStmtUses(3,2);
    PKB::uses->storeStmtUses(4,0);
    PKB::uses->storeStmtUses(4,1);
    PKB::uses->storeStmtUses(6,1);
    PKB::uses->storeStmtUses(6,2);
    PKB::uses->storeStmtUses(9,0);
    PKB::uses->storeStmtUses(10,1);
    PKB::uses->storeStmtUses(11,0);
    PKB::uses->storeStmtUses(11,1);
    PKB::uses->storeStmtUses(11,2);
    PKB::uses->storeStmtUses(12,0);
    PKB::uses->storeStmtUses(12,1);
    PKB::uses->storeStmtUses(12,2);
    PKB::uses->storeStmtUses(13,2);
    PKB::uses->storeStmtUses(15,0);
    PKB::uses->storeStmtUses(16,2);
    PKB::uses->storeStmtUses(17,1);

    PKB::uses->storeProcUses(0,0);
    PKB::uses->storeProcUses(0,1);
    PKB::uses->storeProcUses(0,2);
    PKB::uses->storeProcUses(1,0);
    PKB::uses->storeProcUses(1,1);
    PKB::uses->storeProcUses(1,2);
    PKB::uses->storeProcUses(17,1);

    PKB::modifies->storeStmtModifies(1,0);
    PKB::modifies->storeStmtModifies(1,1);
    PKB::modifies->storeStmtModifies(1,2);
    PKB::modifies->storeStmtModifies(2,1);
    PKB::modifies->storeStmtModifies(3,0);
    PKB::modifies->storeStmtModifies(3,2);
    PKB::modifies->storeStmtModifies(4,2);
    PKB::modifies->storeStmtModifies(5,0);
    PKB::modifies->storeStmtModifies(6,0);
    PKB::modifies->storeStmtModifies(9,0);
    PKB::modifies->storeStmtModifies(10,0);
    PKB::modifies->storeStmtModifies(11,0);
    PKB::modifies->storeStmtModifies(11,2);
    PKB::modifies->storeStmtModifies(12,0);
    PKB::modifies->storeStmtModifies(12,2);
    PKB::modifies->storeStmtModifies(13,2);
    PKB::modifies->storeStmtModifies(15,0);
    PKB::modifies->storeStmtModifies(16,2);
    PKB::modifies->storeStmtModifies(17,0);

    PKB::modifies->storeProcModifies(0,0);
    PKB::modifies->storeProcModifies(0,1);
    PKB::modifies->storeProcModifies(0,2);
    PKB::modifies->storeProcModifies(1,0);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(3,0);

    PKB::populatePKB();
}


TEST_CASE("getAffectsBip Test3") {
    setUpAffectsBipTest3();
    REQUIRE(PKB::affectsBip->getAffectsBip(1) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(2) == unordered_set<ProgLine>{4,6,10,17});
    REQUIRE(PKB::affectsBip->getAffectsBip(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(4) == unordered_set<ProgLine>{6,13,16});
    REQUIRE(PKB::affectsBip->getAffectsBip(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(6) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(8) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(9) == unordered_set<ProgLine>{4});
    REQUIRE(PKB::affectsBip->getAffectsBip(10) == unordered_set<ProgLine>{4});
    REQUIRE(PKB::affectsBip->getAffectsBip(11) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(12) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(13) == unordered_set<ProgLine>{13,16});
    REQUIRE(PKB::affectsBip->getAffectsBip(14) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(15) == unordered_set<ProgLine>{4});
    REQUIRE(PKB::affectsBip->getAffectsBip(16) == unordered_set<ProgLine>{13,16});
    REQUIRE(PKB::affectsBip->getAffectsBip(17) == unordered_set<ProgLine>{15,9});
}

TEST_CASE("getAffectsBipSize Test3") {
    setUpAffectsBipTest3();
    REQUIRE(PKB::affectsBip->getAffectsBipSize() == 16);
}


//    procedure B {
//            01      call C;
//            02      call C;
//            03      call C; }
//
//    procedure C {
//            04      d = a;
//            05      a = b;
//            06      b = c;
//            07      c = d; }

void setUpAffectsBipTest4() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("B"); // 0
    PKB::procTable->storeProcName("C"); // 1
    PKB::procTable->storeProcStmt(0, 1, 3);
    PKB::procTable->storeProcStmt(1, 4, 7);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, ASSIGN_);

    PKB::calls->storeCalls(1, 0, 1);
    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(3, 0, 1);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);

    // a: 0, b:1, c:2, d:3
    PKB::uses->storeStmtUses(4,0);
    PKB::uses->storeStmtUses(5,1);
    PKB::uses->storeStmtUses(6,2);
    PKB::uses->storeStmtUses(7,3);

    PKB::uses->storeProcUses(1,0);
    PKB::uses->storeProcUses(1,1);
    PKB::uses->storeProcUses(1,2);
    PKB::uses->storeProcUses(1,3);

    PKB::modifies->storeStmtModifies(4,3);
    PKB::modifies->storeStmtModifies(5,0);
    PKB::modifies->storeStmtModifies(6,1);
    PKB::modifies->storeStmtModifies(7,2);

    PKB::modifies->storeProcModifies(1,0);
    PKB::modifies->storeProcModifies(1,1);
    PKB::modifies->storeProcModifies(1,2);
    PKB::modifies->storeProcModifies(1,3);

    PKB::populatePKB();
}


TEST_CASE("getAffectsBip Test4") {
    setUpAffectsBipTest4();
    REQUIRE(PKB::affectsBip->getAffectsBip(1) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(2) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBip(4) == unordered_set<ProgLine>{7});
    REQUIRE(PKB::affectsBip->getAffectsBip(5) == unordered_set<ProgLine>{4});
    REQUIRE(PKB::affectsBip->getAffectsBip(6) == unordered_set<ProgLine>{5});
    REQUIRE(PKB::affectsBip->getAffectsBip(7) == unordered_set<ProgLine>{6});
}

TEST_CASE("getAffectsBipSize Test4") {
    setUpAffectsBipTest4();
    REQUIRE(PKB::affectsBip->getAffectsBipSize() == 4);
}

TEST_CASE("getAffectsBipStar Test4") {
    setUpAffectsBipTest4();
    REQUIRE(PKB::affectsBip->getAffectsBipStar(1) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBipStar(2) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBipStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::affectsBip->getAffectsBipStar(4) == unordered_set<ProgLine>{7,6,5});
    REQUIRE(PKB::affectsBip->getAffectsBipStar(5) == unordered_set<ProgLine>{4,7,6});
    REQUIRE(PKB::affectsBip->getAffectsBipStar(6) == unordered_set<ProgLine>{5,4,7});
    REQUIRE(PKB::affectsBip->getAffectsBipStar(7) == unordered_set<ProgLine>{6,5});
}

TEST_CASE("getAffectsBipStarSize Test4") {
    setUpAffectsBipTest4();
    REQUIRE(PKB::affectsBip->getAffectsBipStarSize() == 11);
}