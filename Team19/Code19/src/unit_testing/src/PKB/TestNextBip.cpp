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

TEST_CASE("populateNextBip Test") {
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
