#include "PKB/PKB.h"
#include "PKB/Calls.h"

#include "catch.hpp"
using namespace std;

//    E.g. p has ID 1, q has ID 2
//         a has ID 1, b has ID 2, c has ID 3, d has ID 4
//       procedure p {
//    1.     call q; --> Uses(p, b), Modifies(p, a), Uses(1, b), Modifies(1, a), Uses(p, d), Modifies(p, c), Uses(1, d), Modifies(1, c)
//       }
//
//       procedure q {
//    2.     a = b + 1; --> Uses(q, b), Modifies(q, a), Uses(2, b), Modifies(2, a)
//    3.     call r;    --> Uses(q, d), Modifies(q, c), Uses(3, d), Modifies(3, c)
//       }
//
//       procedure r {
//    4.     c = d;     --> Uses(r, d), Modifies(r, c), Uses(4, d), Modifies(4, c)
//       }

void setUpCallsTest() {
    PKB::resetPKB();
    PKB::calls->storeCalls(1, 1, 2);
    PKB::calls->storeCalls(3, 2, 3);
    PKB::uses->storeStmtUses(2, 2);          // statement 2 uses b
    PKB::uses->storeProcUses(2, 2);           // procedure q uses b
    PKB::uses->storeStmtUses(4, 4);         // statement 4 uses d
    PKB::uses->storeProcUses(3, 4);          // procedure r uses d
    PKB::modifies->storeStmtModifies(2, 1); // statement 2 modifies a
    PKB::modifies->storeProcModifies(2, 1);   // procedure q modifies a
    PKB::modifies->storeStmtModifies(4, 3);  // statement 4 modifies c
    PKB::modifies->storeProcModifies(3, 3);   // procedure r modifies c
    PKB::calls->processCalls();
}

TEST_CASE("storeCalls Test") {
    Calls* calls = new Calls();
    REQUIRE(calls->storeCalls(1, 1, 2));
    REQUIRE(calls->storeCalls(3, 2, 3));
    REQUIRE_FALSE(calls->storeCalls(1, 4, 5)); // 1 statement can only have 1 call.
    REQUIRE(calls->storeCalls(2, 1, 2));       // Same procedure can call another procedure multiple times
}

TEST_CASE("hasCalls Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->hasCalls(1));
    REQUIRE_FALSE(PKB::calls->hasCalls(2));
    REQUIRE(PKB::calls->hasCalls(3));
    REQUIRE_FALSE(PKB::calls->hasCalls(4));
}

// All tests from now will all test for the logic within processCalls()
TEST_CASE("hasCyclicalCall Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE_FALSE(PKB::calls->hasCyclicalCall());

//    procedure p {
//        call q;
//    };
//
//    procedure q {
//        call r;
//    };
//
//    procedure r {
//        call p;
//    };

    Calls* calls = new Calls();
    calls->storeCalls(1, 1, 2);
    calls->storeCalls(2, 2, 3);
    calls->storeCalls(3, 3, 1);
    REQUIRE_FALSE(calls->processCalls());
    REQUIRE(calls->hasCyclicalCall());
}

TEST_CASE("isCalls Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->isCalls(1,2));
    REQUIRE(PKB::calls->isCalls(2,3));
    REQUIRE_FALSE(PKB::calls->isCalls(1,3));
    REQUIRE_FALSE(PKB::calls->isCalls(3,1));
    REQUIRE_FALSE(PKB::calls->isCalls(2,1));
    REQUIRE_FALSE(PKB::calls->isCalls(3,2));
}

TEST_CASE("isCallsStar Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->isCallsStar(1,2));
    REQUIRE(PKB::calls->isCallsStar(2,3));
    REQUIRE(PKB::calls->isCallsStar(1,3));
    REQUIRE_FALSE(PKB::calls->isCallsStar(3,1));
    REQUIRE_FALSE(PKB::calls->isCallsStar(2,1));
    REQUIRE_FALSE(PKB::calls->isCallsStar(3,2));
}

TEST_CASE("getCalleeInStmt Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCalleeInStmt(1) == 2);
    REQUIRE(PKB::calls->getCalleeInStmt(3) == 3);
    REQUIRE(PKB::calls->getCalleeInStmt(5) == 1);
    REQUIRE(PKB::calls->getCalleeInStmt(6) == 1);
    REQUIRE(PKB::calls->getCalleeInStmt(2) == -1); // Not a call statement
}

TEST_CASE("getStmtNumThatCallsCallee Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getStmtNumThatCallsCallee(2) == unordered_set<StmtNum>({1}));
    REQUIRE(PKB::calls->getStmtNumThatCallsCallee(3) == unordered_set<StmtNum>({3}));
    REQUIRE(PKB::calls->getStmtNumThatCallsCallee(1) == unordered_set<StmtNum>({5,6}));
    REQUIRE(PKB::calls->getStmtNumThatCallsCallee(4).empty());
}

TEST_CASE("getCallers Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 2); // suppose we have another procedure s that also calls procedure q.
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallers(1).empty());    // no procedures call p
    REQUIRE(PKB::calls->getCallers(2) == unordered_set<ID>({1, 4})); // procedure p and s calls q
    REQUIRE(PKB::calls->getCallers(3) == unordered_set<ID>({2}));    // only procedure q calls r
}

TEST_CASE("getCallees Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 3, 4);
    PKB::calls->storeCalls(6, 3, 5); // Suppose procedure r calls 2 more procedures
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallees(1) == unordered_set<ID>({2})); // procedure p calls q only
    REQUIRE(PKB::calls->getCallees(2) == unordered_set<ID>({3})); // procedure q calls r only
    REQUIRE(PKB::calls->getCallees(3) == unordered_set<ID>({4,5})); // procedure r calls 2 more procedures
    REQUIRE(PKB::calls->getCallees(4).empty());
}

TEST_CASE("getCallersStar Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose we have another procedure s that also calls procedure p and calls it twice (stmt 5 and 6)
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallersStar(1) == unordered_set<ID>({4})); // procedure s calls p
    REQUIRE(PKB::calls->getCallersStar(2) == unordered_set<ID>({1, 4})); // procedure p calls q so s calls* q
    REQUIRE(PKB::calls->getCallersStar(3) == unordered_set<ID>({2, 1, 4}));    // procedure q calls r, so procedure p and s calls* r
    REQUIRE(PKB::calls->getCallersStar(4).empty()); // no procedure calls/calls* s
}

TEST_CASE("getCalleesStar Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose we have another procedure s that also calls procedure p.
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCalleesStar(1) == unordered_set<ID>({2, 3})); // procedure p calls q and calls* r
    REQUIRE(PKB::calls->getCalleesStar(2) == unordered_set<ID>({3})); // procedure q calls r only
    REQUIRE(PKB::calls->getCalleesStar(3).empty()); // procedure r does not call any procedures
    REQUIRE(PKB::calls->getCalleesStar(4) == unordered_set<ID>({1, 2, 3})); // s calls p, thus it calls* q and r
}

TEST_CASE("getStmtsOfCalls test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getStmtsOfCalls(1, 2) == unordered_set<StmtNum>({1})); // p only call q in stmt 1
    REQUIRE(PKB::calls->getStmtsOfCalls(2, 3) == unordered_set<StmtNum>({3})); // q only call r in stmt 3
    REQUIRE(PKB::calls->getStmtsOfCalls(4, 1) == unordered_set<StmtNum>({5, 6})); // s calls p in stmt 5 and 6
}

TEST_CASE("getCallsSize Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallsSize() == 2);
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallsSize() == 3);
}

TEST_CASE("getCallsStarSize Test") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallsStarSize() == 3);
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::calls->getCallsStarSize() == 6);
}

TEST_CASE("getAllCalls Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    pair<vector<ID>, vector<ID> > result = PKB::calls->getAllCalls();
    vector<ID> ps = result.first;
    vector<ID> qs = result.second;
    int num_pairs = ps.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == PKB::calls->getCallsSize());
    REQUIRE(qs.size() == num_pairs);
    // check that each pair at the same index has Calls relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(PKB::calls->isCalls(ps.at(i), qs.at(i)));
    }
}

TEST_CASE("getAllCallsStar Test") {
    setUpCallsTest();
    PKB::calls->storeCalls(5, 4, 1); // suppose procedure s calls procedure p twice (stmt 5 and stmt 6).
    PKB::calls->storeCalls(6, 4, 1);
    REQUIRE(PKB::calls->processCalls());
    pair<vector<ID>, vector<ID> > result = PKB::calls->getAllCallsStar();
    vector<ID> ps = result.first;
    vector<ID> qs = result.second;
    int num_pairs = ps.size();
    // Check that it has correct number of pairs
    REQUIRE(num_pairs == PKB::calls->getCallsStarSize());
    REQUIRE(qs.size() == num_pairs);
    // check that each pair at the same index has Calls relationship
    for (int i = 0; i < num_pairs; i++) {
        REQUIRE(PKB::calls->isCallsStar(ps.at(i), qs.at(i)));
    }
}

TEST_CASE("check uses and modifies") {
    setUpCallsTest();
    REQUIRE(PKB::calls->processCalls());
    REQUIRE(PKB::uses->getProcsUses(1).empty()); // no procedure uses a
    REQUIRE(PKB::uses->getProcsUses(2) == unordered_set<ID>({1, 2})); // procedure p and q use b
    REQUIRE(PKB::uses->getProcsUses(3).empty()); // no procedure uses c
    REQUIRE(PKB::uses->getProcsUses(4) == unordered_set<ID>({1, 2, 3})); // all procedure use d

    REQUIRE(PKB::uses->getStmtsUses(1).empty()); // no statements uses a
    REQUIRE(PKB::uses->getStmtsUses(2) == unordered_set<ID>({1, 2})); // statements 1 and 2 use b
    REQUIRE(PKB::uses->getStmtsUses(3).empty()); // no statements uses c
    REQUIRE(PKB::uses->getStmtsUses(4) == unordered_set<ID>({1, 3, 4})); // statements 1,3,4 use d

    REQUIRE(PKB::modifies->getProcsModifies(1) == unordered_set<ID>({1, 2})); // procedure p and q modify a
    REQUIRE(PKB::modifies->getProcsModifies(2).empty()); // no procedure modifies b
    REQUIRE(PKB::modifies->getProcsModifies(3) == unordered_set<ID>({1, 2, 3})); // all procedures modify c
    REQUIRE(PKB::modifies->getProcsModifies(4).empty()); // no procedure modifies d

    REQUIRE(PKB::modifies->getStmtsModifies(1) == unordered_set<ID>({1, 2})); // statement 1 and 2 modify a
    REQUIRE(PKB::modifies->getStmtsModifies(2).empty()); // no statement modifies b
    REQUIRE(PKB::modifies->getStmtsModifies(3) == unordered_set<ID>({1, 3, 4})); // statement 1,3,4 modify c
    REQUIRE(PKB::modifies->getStmtsModifies(4).empty()); // no statement modifies d
}

TEST_CASE("check uses and modifies – with containers") {
//       procedure p {
//    1.     while (d == 3) { --> Uses(1, d), Uses(p, d), Uses(1, e), Uses(p, e), Uses(p, b), Modifies(p, a), Uses(1, b), Modifies(1, a), Modifies(p, c), Modifies(1, c)
//    2.         if (e == 2) { --> Uses(2, e), Uses(p, e), Uses(p, b), Modifies(p, a), Uses(2, b), Modifies(2, a), Modifies(p, c), Modifies(2, c)
//    3.             call q; --> Uses(p, b), Modifies(p, a), Uses(3, b), Modifies(3, a), Modifies(p, c), Modifies(3, c)
//               }
//           }
//       }
//
//       procedure q {
//    4.     a = b + 1; --> Uses(q, b), Modifies(q, a), Uses(4, b), Modifies(4, a)
//    5.     call r;    --> Modifies(q, c), Modifies(5, c)
//       }
//
//       procedure r {
//    6.     c = 1;     --> Modifies(r, c), Modifies(6, c)
//       }
    PKB::resetPKB();
    PKB::calls->storeCalls(3, 1, 2);
    PKB::calls->storeCalls(5, 2, 3);
    PKB::parent->storeParent(1, 2);
    PKB::parent->storeParent(2, 3);
    PKB::uses->storeStmtUses(1, 4);          // statement 1 uses d
    PKB::uses->storeProcUses(1, 4);           // procedure p uses d
    PKB::uses->storeStmtUses(2, 5);          // statement 2 uses e
    PKB::uses->storeProcUses(1, 5);           // procedure p uses e
    PKB::uses->storeStmtUses(1, 5);          // statement 1 uses e [handled by Design Extractor]
    PKB::uses->storeStmtUses(4, 2);          // statement 4 uses b
    PKB::uses->storeProcUses(2, 2);           // procedure q uses b
    PKB::modifies->storeStmtModifies(4, 1); // statement 4 modifies a
    PKB::modifies->storeProcModifies(2, 1);   // procedure q modifies a
    PKB::modifies->storeStmtModifies(6, 3);  // statement 6 modifies c
    PKB::modifies->storeProcModifies(3, 3);   // procedure r modifies c
    PKB::populatePKB();

    REQUIRE(PKB::uses->getProcsUses(1).empty()); // no procedure uses a
    REQUIRE(PKB::uses->getProcsUses(2) == unordered_set<ID>({1, 2})); // procedure p and q use b
    REQUIRE(PKB::uses->getProcsUses(3).empty()); // no procedure uses c
    REQUIRE(PKB::uses->getProcsUses(4) == unordered_set<ID>({1})); // procedure p uses d
    REQUIRE(PKB::uses->getProcsUses(5) == unordered_set<ID>({1})); // procedure p uses e

    REQUIRE(PKB::uses->getStmtsUses(1).empty()); // no statements uses a
    REQUIRE(PKB::uses->getStmtsUses(2) == unordered_set<ID>({1, 2, 3, 4})); // statements 1, 2, 3, 4 use b
    REQUIRE(PKB::uses->getStmtsUses(3).empty()); // no statements uses c
    REQUIRE(PKB::uses->getStmtsUses(4) == unordered_set<ID>({1})); // statements 1 use d
    REQUIRE(PKB::uses->getStmtsUses(5) == unordered_set<ID>({1,2})); // statement 1,2 uses e

    REQUIRE(PKB::modifies->getProcsModifies(1) == unordered_set<ID>({1, 2})); // procedure p and q modify a
    REQUIRE(PKB::modifies->getProcsModifies(2).empty()); // no procedure modifies b
    REQUIRE(PKB::modifies->getProcsModifies(3) == unordered_set<ID>({1, 2, 3})); // all procedures modify c
    REQUIRE(PKB::modifies->getProcsModifies(4).empty()); // no procedure modifies d
    REQUIRE(PKB::modifies->getProcsModifies(5).empty()); // no procedure modifies e

    REQUIRE(PKB::modifies->getStmtsModifies(1) == unordered_set<ID>({1, 2, 3, 4})); // statement 1, 2, 3, 4 modify a
    REQUIRE(PKB::modifies->getStmtsModifies(2).empty()); // no statement modifies b
    REQUIRE(PKB::modifies->getStmtsModifies(3) == unordered_set<ID>({1, 2, 3, 5, 6})); // statement 1,2,3,5,6 modify c
    REQUIRE(PKB::modifies->getStmtsModifies(4).empty()); // no statement modifies d
    REQUIRE(PKB::modifies->getStmtsModifies(5).empty()); // no statement modifies e
}
