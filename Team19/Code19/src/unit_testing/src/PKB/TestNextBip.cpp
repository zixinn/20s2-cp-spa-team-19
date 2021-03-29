#include "PKB/PKB.h"
#include "PKB/NextBip.h"

#include "catch.hpp"
using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
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

NextBip* setUpNextBipTest() {
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

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(8, 10);
    PKB::next->storeNext(10, 11);
    PKB::next->storeNext(10, 12);

    PKB::next->populateNextStar();
    PKB::calls->processCalls();
    NextBip* nextBip = new NextBip();
    nextBip->populateNextBipAndNextBipStar();
    return nextBip;
}

TEST_CASE("getNextBip Test") {
    NextBip* nextBip = setUpNextBipTest();
    REQUIRE(nextBip->getNextBip(1) == unordered_set<ProgLine>{2});
    REQUIRE(nextBip->getNextBip(2) == unordered_set<ProgLine>{6});
    REQUIRE(nextBip->getNextBip(3) == unordered_set<ProgLine>{4});
    REQUIRE(nextBip->getNextBip(4) == unordered_set<ProgLine>{8});
    REQUIRE(nextBip->getNextBip(5) == unordered_set<ProgLine>{ });
    REQUIRE(nextBip->getNextBip(6) == unordered_set<ProgLine>{7});
    REQUIRE(nextBip->getNextBip(7) == unordered_set<ProgLine>{8});
    REQUIRE(nextBip->getNextBip(8) == unordered_set<ProgLine>{9, 10});
    REQUIRE(nextBip->getNextBip(9) == unordered_set<ProgLine>{3, 5});
    REQUIRE(nextBip->getNextBip(10) == unordered_set<ProgLine>{11, 12});
    REQUIRE(nextBip->getNextBip(11) == unordered_set<ProgLine>{3, 5});
    REQUIRE(nextBip->getNextBip(12) == unordered_set<ProgLine>{3, 5});
}

TEST_CASE("isNextBip Test") {
    NextBip* nextBip = setUpNextBipTest();
    REQUIRE(nextBip->isNextBip(1,2));
    REQUIRE(nextBip->isNextBip(2,6));
    REQUIRE(nextBip->isNextBip(3,4));
    REQUIRE(nextBip->isNextBip(4,8));
    REQUIRE_FALSE(nextBip->isNextBip(5,6));
    REQUIRE(nextBip->isNextBip(6,7));
    REQUIRE(nextBip->isNextBip(7,8));
    REQUIRE(nextBip->isNextBip(8,9));
    REQUIRE(nextBip->isNextBip(8,10));
    REQUIRE(nextBip->isNextBip(9,3));
    REQUIRE(nextBip->isNextBip(9,5));
    REQUIRE(nextBip->isNextBip(10,11));
    REQUIRE(nextBip->isNextBip(10,12));
    REQUIRE(nextBip->isNextBip(11,3));
    REQUIRE(nextBip->isNextBip(11,5));
    REQUIRE(nextBip->isNextBip(12,3));
    REQUIRE(nextBip->isNextBip(12,5));
}

TEST_CASE("getPreviousBip Test") {
    NextBip* nextBip = setUpNextBipTest();
    REQUIRE(nextBip->getPreviousBip(1) == unordered_set<ProgLine>({ }));
    REQUIRE(nextBip->getPreviousBip(2) == unordered_set<ProgLine>{1});
    REQUIRE(nextBip->getPreviousBip(3) == unordered_set<ProgLine>{9,11,12});
    REQUIRE(nextBip->getPreviousBip(4) == unordered_set<ProgLine>{3});
    REQUIRE(nextBip->getPreviousBip(5) == unordered_set<ProgLine>{9,11,12});
    REQUIRE(nextBip->getPreviousBip(6) == unordered_set<ProgLine>{2});
    REQUIRE(nextBip->getPreviousBip(7) == unordered_set<ProgLine>{6});
    REQUIRE(nextBip->getPreviousBip(8) == unordered_set<ProgLine>{4,7});
    REQUIRE(nextBip->getPreviousBip(9) == unordered_set<ProgLine>{8});
    REQUIRE(nextBip->getPreviousBip(10) == unordered_set<ProgLine>{8});
    REQUIRE(nextBip->getPreviousBip(11) == unordered_set<ProgLine>{10});
    REQUIRE(nextBip->getPreviousBip(12) == unordered_set<ProgLine>{10});
}

TEST_CASE("getNextBipSize Test") {
    NextBip* nextBip = setUpNextBipTest();
    REQUIRE(nextBip->getNextBipSize() == 16);
}

TEST_CASE("getAllNextBip Test") {
    NextBip* nextBip = setUpNextBipTest();
    pair<vector<ID>, vector<ID> > result = nextBip->getAllNextBip();
    vector<ID> n1s = result.first;
    vector<ID> n2s = result.second;
    int num_pairs = n1s.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == nextBip->getNextBipSize());
    REQUIRE(n2s.size() == num_pairs);
    // check that each pair at the same index has Calls relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(nextBip->isNextBip(n1s.at(i), n2s.at(i)));
    }
}

TEST_CASE("getNextBipStar Test") {
    NextBip* nextBip = setUpNextBipTest();
    REQUIRE(nextBip->getNextBipStar(1) == unordered_set<ProgLine>{2,3,4,5,6,7,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(2) == unordered_set<ProgLine>{3,4,5,6,7,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(3) == unordered_set<ProgLine>{4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(4) == unordered_set<ProgLine>{5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(5) == unordered_set<ProgLine>{});
    REQUIRE(nextBip->getNextBipStar(6) == unordered_set<ProgLine>{3,4,5,7,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(7) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(8) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(9) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(10) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(11) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
    REQUIRE(nextBip->getNextBipStar(12) == unordered_set<ProgLine>{3,4,5,8,9,10,11,12});
}

//    procedure first {
//    01    a=1;
//    02    call second;
//    }
//
//    procedure second {
//    03    b=2;
//    04    while (a == c) {
//    05        call third;
//          }
//    06    call fourth;
//    }
//
//    procedure third {
//    07    if (b == 5) then {
//    08        d = d;
//    09        call fourth;
//          } else {
//    10        e = e;
//        }
//    }
//
//    procedure fourth {
//    11    f = f;
//    };

NextBip* setUpNextBipTest2() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("first"); // 0
    PKB::procTable->storeProcName("second"); // 1
    PKB::procTable->storeProcName("third"); // 2
    PKB::procTable->storeProcName("fourth"); // 3
    PKB::procTable->storeProcStmt(0, 1, 2);
    PKB::procTable->storeProcStmt(1, 3, 6);
    PKB::procTable->storeProcStmt(2, 7, 7);
    PKB::procTable->storeProcStmt(3, 11, 11);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(7, 8, set<StmtNum>{9});
    PKB::stmtTable->storeElseStmt(7, 10, set<StmtNum>{10});

    PKB::calls->storeCalls(2, 0, 1);
    PKB::calls->storeCalls(5, 1, 2);
    PKB::calls->storeCalls(6, 1, 3);
    PKB::calls->storeCalls(9, 2, 3);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 4);
    PKB::next->storeNext(4, 6);
    PKB::next->storeNext(7, 8);
    PKB::next->storeNext(8, 9);
    PKB::next->storeNext(7, 10);

    PKB::next->populateNextStar();
    PKB::calls->processCalls();
    NextBip* nextBip = new NextBip();
    nextBip->populateNextBipAndNextBipStar();
    return nextBip;
}

TEST_CASE("getNextBip Test2") {
    NextBip* nextBip = setUpNextBipTest2();
    REQUIRE(nextBip->getNextBip(1) == unordered_set<ProgLine>{2});
    REQUIRE(nextBip->getNextBip(2) == unordered_set<ProgLine>{3});
    REQUIRE(nextBip->getNextBip(3) == unordered_set<ProgLine>{4});
    REQUIRE(nextBip->getNextBip(4) == unordered_set<ProgLine>{5,6});
    REQUIRE(nextBip->getNextBip(5) == unordered_set<ProgLine>{7});
    REQUIRE(nextBip->getNextBip(6) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBip(7) == unordered_set<ProgLine>{8,10});
    REQUIRE(nextBip->getNextBip(8) == unordered_set<ProgLine>{9});
    REQUIRE(nextBip->getNextBip(9) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBip(10) == unordered_set<ProgLine>{4});
    REQUIRE(nextBip->getNextBip(11) == unordered_set<ProgLine>{4});
}

TEST_CASE("getNextBipStar Test2") {
    NextBip* nextBip = setUpNextBipTest2();
    REQUIRE(nextBip->getNextBipStar(1) == unordered_set<ProgLine>{2,3,4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(2) == unordered_set<ProgLine>{3,4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(3) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(4) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(5) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(6) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBipStar(7) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(8) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(9) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(10) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
    REQUIRE(nextBip->getNextBipStar(11) == unordered_set<ProgLine>{4,5,6,7,8,9,10,11});
}


//    procedure a {
//    01    call b;
//    }
//
//    procedure b {
//    02    if (a == 1) then {
//    03        c = 1;
//    04        if (b == 1) then {
//    05            call c;
//              } else {
//    06            while (c == 1) {
//    07                call d;
//                  }
//    08            call e;
//              }
//          } else {
//    09        call d;
//          }
//    }
//
//    procedure c {
//    10    call d;
//    }
//
//    procedure d {
//    11    if (f == 1) then {
//    12        call e;
//    13        f = f + 1;
//          } else {
//    14        f = f + 1;
//          }
//    15    g = g + 1;
//    }
//
//    procedure e {
//    16    f = f + 1;
//    }

NextBip* setUpNextBipTest3() {
    PKB::resetPKB();

    PKB::procTable->storeProcName("a"); // 0
    PKB::procTable->storeProcName("b"); // 1
    PKB::procTable->storeProcName("c"); // 2
    PKB::procTable->storeProcName("d"); // 3
    PKB::procTable->storeProcName("e"); // 4
    PKB::procTable->storeProcStmt(0, 1, 1);
    PKB::procTable->storeProcStmt(1, 2, 2);
    PKB::procTable->storeProcStmt(2, 10, 10);
    PKB::procTable->storeProcStmt(3, 11, 15);
    PKB::procTable->storeProcStmt(4, 16, 16);

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
    PKB::stmtTable->storeStmt(1, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(9, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(10, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(11, stmtNodeStub, IF_);
    PKB::stmtTable->storeStmt(12, stmtNodeStub, CALL_);
    PKB::stmtTable->storeStmt(13, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(14, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(15, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeIfStmt(2, 3, set<StmtNum>{5,8});
    PKB::stmtTable->storeElseStmt(2, 9, set<StmtNum>{9});
    PKB::stmtTable->storeIfStmt(4, 5, set<StmtNum>{5});
    PKB::stmtTable->storeElseStmt(4, 6, set<StmtNum>{8});
    PKB::stmtTable->storeIfStmt(11, 12, set<StmtNum>{13});
    PKB::stmtTable->storeElseStmt(11, 14, set<StmtNum>{14});

    PKB::calls->storeCalls(1, 0, 1);
    PKB::calls->storeCalls(5, 1, 2);
    PKB::calls->storeCalls(7, 1, 3);
    PKB::calls->storeCalls(8, 1, 4);
    PKB::calls->storeCalls(9, 1, 3);
    PKB::calls->storeCalls(10, 2, 3);
    PKB::calls->storeCalls(12, 3, 4);

    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(2, 9);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(4, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(7, 6);
    PKB::next->storeNext(6, 8);
    PKB::next->storeNext(11, 12);
    PKB::next->storeNext(11, 14);
    PKB::next->storeNext(12, 13);
    PKB::next->storeNext(13, 15);
    PKB::next->storeNext(14, 15);

    PKB::next->populateNextStar();
    PKB::calls->processCalls();
    NextBip* nextBip = new NextBip();
    nextBip->populateNextBipAndNextBipStar();
    return nextBip;
}

TEST_CASE("getNextBip Test3") {
    NextBip* nextBip = setUpNextBipTest3();
    REQUIRE(nextBip->getNextBip(1) == unordered_set<ProgLine>{2});
    REQUIRE(nextBip->getNextBip(2) == unordered_set<ProgLine>{3,9});
    REQUIRE(nextBip->getNextBip(3) == unordered_set<ProgLine>{4});
    REQUIRE(nextBip->getNextBip(4) == unordered_set<ProgLine>{5,6});
    REQUIRE(nextBip->getNextBip(5) == unordered_set<ProgLine>{10});
    REQUIRE(nextBip->getNextBip(6) == unordered_set<ProgLine>{7,8});
    REQUIRE(nextBip->getNextBip(7) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBip(8) == unordered_set<ProgLine>{16});
    REQUIRE(nextBip->getNextBip(9) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBip(10) == unordered_set<ProgLine>{11});
    REQUIRE(nextBip->getNextBip(11) == unordered_set<ProgLine>{12,14});
    REQUIRE(nextBip->getNextBip(12) == unordered_set<ProgLine>{16});
    REQUIRE(nextBip->getNextBip(13) == unordered_set<ProgLine>{15});
    REQUIRE(nextBip->getNextBip(14) == unordered_set<ProgLine>{15});
    REQUIRE(nextBip->getNextBip(15) == unordered_set<ProgLine>{6});
    REQUIRE(nextBip->getNextBip(16) == unordered_set<ProgLine>{13});
}

TEST_CASE("getNextBipStar Test3") {
    NextBip* nextBip = setUpNextBipTest3();
    REQUIRE(nextBip->getNextBipStar(1) == unordered_set<ProgLine>{2,3,4,5,6,7,8,9,10,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(2) == unordered_set<ProgLine>{3,4,5,6,7,8,9,10,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(3) == unordered_set<ProgLine>{4,5,6,7,8,10,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(4) == unordered_set<ProgLine>{5,6,7,8,10,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(5) == unordered_set<ProgLine>{10,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(6) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(7) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(8) == unordered_set<ProgLine>{16});
    REQUIRE(nextBip->getNextBipStar(9) == unordered_set<ProgLine>{11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(10) == unordered_set<ProgLine>{11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(11) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(12) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(13) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(14) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(15) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
    REQUIRE(nextBip->getNextBipStar(16) == unordered_set<ProgLine>{6,7,8,11,12,13,14,15,16});
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

NextBip* setUpNextBipTest4() {
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

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(7, 8);
    PKB::next->storeNext(9, 10);
    PKB::next->storeNext(9, 11);

    PKB::next->populateNextStar();
    PKB::calls->processCalls();
    NextBip* nextBip = new NextBip();
    nextBip->populateNextBipAndNextBipStar();
    return nextBip;
}


TEST_CASE("getNextBip Test4") {
    NextBip* nextBip = setUpNextBipTest4();
    REQUIRE(nextBip->getNextBip(1) == unordered_set<ProgLine>{2});
    REQUIRE(nextBip->getNextBip(2) == unordered_set<ProgLine>{6});
    REQUIRE(nextBip->getNextBip(3) == unordered_set<ProgLine>{4});
    REQUIRE(nextBip->getNextBip(4) == unordered_set<ProgLine>{9});
    REQUIRE(nextBip->getNextBip(5) == unordered_set<ProgLine>{ });
    REQUIRE(nextBip->getNextBip(6) == unordered_set<ProgLine>{7});
    REQUIRE(nextBip->getNextBip(7) == unordered_set<ProgLine>{9});
    REQUIRE(nextBip->getNextBip(8) == unordered_set<ProgLine>{3});
    REQUIRE(nextBip->getNextBip(9) == unordered_set<ProgLine>{10,11});
    REQUIRE(nextBip->getNextBip(10) == unordered_set<ProgLine>{5,8});
    REQUIRE(nextBip->getNextBip(11) == unordered_set<ProgLine>{5,8});
}

TEST_CASE("getNextBipStar Test4") {
    NextBip* nextBip = setUpNextBipTest4();
    REQUIRE(nextBip->getNextBipStar(1) == unordered_set<ProgLine>{2,6,7,9,10,8,3,4,11,5});
    REQUIRE(nextBip->getNextBipStar(2) == unordered_set<ProgLine>{6,7,9,10,8,3,4,11,5});
    REQUIRE(nextBip->getNextBipStar(3) == unordered_set<ProgLine>{4,9,10,11,5});
    REQUIRE(nextBip->getNextBipStar(4) == unordered_set<ProgLine>{9,10,11,5});
    REQUIRE(nextBip->getNextBipStar(5) == unordered_set<ProgLine>{ });
    REQUIRE(nextBip->getNextBipStar(6) == unordered_set<ProgLine>{7,9,10,8,3,4,11,5});
    REQUIRE(nextBip->getNextBipStar(7) == unordered_set<ProgLine>{9,10,8,3,4,11,5});
    REQUIRE(nextBip->getNextBipStar(8) == unordered_set<ProgLine>{3,4,9,10,11,5});
    REQUIRE(nextBip->getNextBipStar(9) == unordered_set<ProgLine>{10,11,8,3,4,9,5});
    REQUIRE(nextBip->getNextBipStar(10) == unordered_set<ProgLine>{8,3,4,9,10,11,5});
    REQUIRE(nextBip->getNextBipStar(11) == unordered_set<ProgLine>{8,3,4,9,10,11,5});
}