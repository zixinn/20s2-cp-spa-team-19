#include "catch.hpp"

#include "AST/Index.h"
#include "SP/Parser.h"
#include "PKB/PKB.h"

using namespace std;

// Tests the Parser's ability to parse inputs and DE's ability to extract information correctly.
// Uses the PKB because there is no way to see DE's outputs otherwise.
TEST_CASE("Simple - Parser-DE Test") {
    /*
    procedure star {
     1   x = 1 + 2 * a;
      }
     */
    string input = "procedure star {x = 1 + 2 * a;} ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");

    // Check consts used in AssignStmt
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("3") == false);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == -1); // no Followers

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ varID1 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1});
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{ });
}

TEST_CASE("Complicated Expression - Parser-DE Test") {
    /*
    procedure star {
      1   x = (1) + (2 * ((5 * 0 * 2 + 5 / 1) * a) / 20 * (1097 - pollution));
     }
     */
    string input = "procedure star {x = (1) + (2 * ((5 * 0 * 2 + 5 / 1) * a) / 20 * (1097 - pollution));} ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");
    ID varID2 = PKB::varTable->getVarID("pollution");
    REQUIRE(varID2 == 2);
    REQUIRE(PKB::varTable->getVarName(varID2) == "pollution");

    // Check consts used in AssignStmt
    REQUIRE(PKB::constTable->hasConst("0") == true);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->hasConst("20") == true);
    REQUIRE(PKB::constTable->hasConst("1097") == true);
    REQUIRE(PKB::constTable->hasConst("3") == false);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == -1); // no Followers

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ varID1, varID2 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{ });
}

TEST_CASE("Nested while-if test - Parser-DE Test") {
    /*
    procedure star {
     1   x = 1 + 2 * a;
     2   while (v > 1) {
     3       x = 2;
     4       if (v == 1) then {
     5           c = a;
            } else {
     6          b = 2;
            }
        }
     7   print p;
     8   read s;
      }
     */
    string input = "procedure star {x = 1 + 2 * a; while (v > 1) {x = 2; if (v==1)then{c = a;}else{b = 2;}} print p; read s;} ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");
    ID varID2 = PKB::varTable->getVarID("c");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "c");
    ID varID3 = PKB::varTable->getVarID("b");
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "b");
    ID varID4 = PKB::varTable->getVarID("p");
    REQUIRE(varID4 == 5);
    REQUIRE(PKB::varTable->getVarName(varID4) == "p");
    ID varID5 = PKB::varTable->getVarID("s");
    REQUIRE(varID5 == 6);
    REQUIRE(PKB::varTable->getVarName(varID5) == "s");

    ID condVarId = PKB::varTable->getVarID("v");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "v");

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("3") == false);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);
    REQUIRE(PKB::follows->getFollower(2) != 5);
    REQUIRE(PKB::follows->getFollower(2) == 7);
    REQUIRE(PKB::follows->getFollower(3) == 4);
    REQUIRE(PKB::follows->getFollower(4) != 5);
    REQUIRE(PKB::follows->getFollower(4) != 6);
    REQUIRE(PKB::follows->getFollower(5) != 6);
    REQUIRE(PKB::follows->getFollower(7) == 8);
    REQUIRE(PKB::follows->getFollower(8) != 8); // stmt cannot follow itself

    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<StmtNum>{ 2, 7, 8 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<StmtNum>{ 7, 8 });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<StmtNum>{ 4 });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<StmtNum>{ 8 });
    REQUIRE(PKB::follows->getFollowerStar(8) == unordered_set<StmtNum>{ });

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(5) == 4);
    REQUIRE(PKB::parent->getParent(6) == 4);
    REQUIRE(PKB::parent->getParent(7) == -1);
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4, 5, 6 });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ 5, 6 });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1, 2, 4, 5 });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ 7 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, varID1, varID4 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2, 3 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<ID>{ 2, 4, 5 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 4, 6 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID5) == unordered_set<ID>{ 8 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{ });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 7 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 4 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{  });

    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ });


}

TEST_CASE("Triple-nested while test - Parser-DE Test") {
    /*
    procedure ToTheMoon {
     1   x = 1 + 2 * a;
     2   while (v > 1) {
     3       x = 2;
     4       while (v == 1) {
     5          c = a;
     6          while (t != 5000) {
     7              b = 2;
                }
            }
        }
     8   print p;
     9   read s;
      }
     */
    string input = "procedure ToTheMoon {x = 1 + 2 * a; while (v > 1) {x = 2; while (v==1) {c = a; while (t != 5000) {b = 2;}}} print p; read s;} ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // actual While stmt
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // 1st While's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // 2nd While's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == true); // 3rd While's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(9) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");
    ID varID2 = PKB::varTable->getVarID("c");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "c");
    ID varID3 = PKB::varTable->getVarID("b");
    REQUIRE(varID3 == 5);
    REQUIRE(PKB::varTable->getVarName(varID3) == "b");
    ID varID4 = PKB::varTable->getVarID("p");
    REQUIRE(varID4 == 6);
    REQUIRE(PKB::varTable->getVarName(varID4) == "p");
    ID varID5 = PKB::varTable->getVarID("s");
    REQUIRE(varID5 == 7);
    REQUIRE(PKB::varTable->getVarName(varID5) == "s");

    ID condVarId = PKB::varTable->getVarID("v");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "v");
    ID condVarId2 = PKB::varTable->getVarID("t");
    REQUIRE(condVarId2 == 4);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "t");

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("5000") == true);

    // Check Follows/Follows*
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);
    REQUIRE(PKB::follows->getFollower(2) != 5);
    REQUIRE(PKB::follows->getFollower(2) != 6);
    REQUIRE(PKB::follows->getFollower(2) != 7);
    REQUIRE(PKB::follows->getFollower(2) == 8);
    REQUIRE(PKB::follows->getFollower(3) == 4);
    REQUIRE(PKB::follows->getFollower(4) != 5);
    REQUIRE(PKB::follows->getFollower(4) != 6);
    REQUIRE(PKB::follows->getFollower(4) != 7);
    REQUIRE(PKB::follows->getFollower(5) == 6);
    REQUIRE(PKB::follows->getFollower(6) != 7);
    REQUIRE(PKB::follows->getFollower(7) != 8);
    REQUIRE(PKB::follows->getFollower(8) == 9);
    REQUIRE(PKB::follows->getFollower(1) != 1);
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<StmtNum>{ 2, 8, 9 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<StmtNum>{ 8, 9 });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<StmtNum>{ 4 });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<StmtNum>{ 6 });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(8) == unordered_set<StmtNum>{ 9 });
    REQUIRE(PKB::follows->getFollowerStar(9) == unordered_set<StmtNum>{ });

    // Check Parent/Parent*
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6 });
    REQUIRE(PKB::parent->getChildren(6) == unordered_set<StmtNum>{ 7 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(5) == 4);
    REQUIRE(PKB::parent->getParent(7) == 6);
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ 5, 6, 7 });
    REQUIRE(PKB::parent->getChildrenStar(6) == unordered_set<StmtNum>{ 7 });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2, 4, 6 });
    REQUIRE(PKB::uses->getStmtsUses(varID) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1, 2, 4, 5 }); // Container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ 8 });
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2, varID1, varID4 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2, 3 }); // Container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{  });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<ID>{ 2, 4, 5 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 4, 6, 7 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID5) == unordered_set<ID>{ 9 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 8 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 4 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 2 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 6 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7, 4 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 6 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(9) == unordered_set<ProgLine>{  });

    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNextStar(9) == unordered_set<ProgLine>{ });
}

TEST_CASE("Triple-nested if test - Parser-DE Test") {
    /*
    procedure ToTheMoon {
     1   x = 1 + 2 * a;
     2   if (v > 1) then {
     3       x = 2;
         } else {
     4       if (v == 1) then {
     5          c = a;
             } else {
     6          if (t != 5000) then {
     7              b = 2;
                } else {
     8              xor = (5 * 2) + 7;
                }
            }
        }
     9   print p;
    10   read s;
      }
     */
    string input = "procedure ToTheMoon {x = 1 + 2 * a; if (v > 1) then {x = 2;} else { "
                        "if (v==1) then {c = a;} else {"
                            "if (t != 5000) then {b = 2;} else {xor = (5 * 2) + 7;}}} print p; read s;} ";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // actual if stmt
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // 1st If's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // 1st If's Else stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // 2nd If's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true); // 2nd If's Else stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == true); // 3rd If's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == true); // 3rd If's Else stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(9) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(10) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");
    ID varID2 = PKB::varTable->getVarID("c");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "c");
    ID varID3 = PKB::varTable->getVarID("b");
    REQUIRE(varID3 == 5);
    REQUIRE(PKB::varTable->getVarName(varID3) == "b");
    ID varID4 = PKB::varTable->getVarID("xor");
    REQUIRE(varID4 == 6);
    REQUIRE(PKB::varTable->getVarName(varID4) == "xor");
    ID varID5 = PKB::varTable->getVarID("p");
    REQUIRE(varID5 == 7);
    REQUIRE(PKB::varTable->getVarName(varID5) == "p");
    ID varID6 = PKB::varTable->getVarID("s");
    REQUIRE(varID6 == 8);
    REQUIRE(PKB::varTable->getVarName(varID6) == "s");

    ID condVarId = PKB::varTable->getVarID("v");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "v");
    ID condVarId2 = PKB::varTable->getVarID("t");
    REQUIRE(condVarId2 == 4);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "t");

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("5000") == true);
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->hasConst("7") == true);

    // Check Follows/Follows*
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);
    REQUIRE(PKB::follows->getFollower(2) != 5);
    REQUIRE(PKB::follows->getFollower(2) != 6);
    REQUIRE(PKB::follows->getFollower(2) != 7);
    REQUIRE(PKB::follows->getFollower(2) != 8);
    REQUIRE(PKB::follows->getFollower(2) == 9);
    REQUIRE(PKB::follows->getFollower(2) != 10);
    REQUIRE(PKB::follows->getFollower(3) != 4);
    REQUIRE(PKB::follows->getFollower(3) != 5);
    REQUIRE(PKB::follows->getFollower(3) != 6);
    REQUIRE(PKB::follows->getFollower(3) != 7);
    REQUIRE(PKB::follows->getFollower(3) != 8);
    REQUIRE(PKB::follows->getFollower(4) != 5);
    REQUIRE(PKB::follows->getFollower(4) != 6);
    REQUIRE(PKB::follows->getFollower(4) != 7);
    REQUIRE(PKB::follows->getFollower(5) != 8);
    REQUIRE(PKB::follows->getFollower(5) != 6);
    REQUIRE(PKB::follows->getFollower(5) != 7);
    REQUIRE(PKB::follows->getFollower(5) != 8);
    REQUIRE(PKB::follows->getFollower(6) != 7);
    REQUIRE(PKB::follows->getFollower(6) != 8);
    REQUIRE(PKB::follows->getFollower(7) != 8);
    REQUIRE(PKB::follows->getFollower(8) != 9);
    REQUIRE(PKB::follows->getFollower(1) != 0);

    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<StmtNum>{ 2, 9, 10 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<StmtNum>{ 9, 10 });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(8) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::follows->getFollowerStar(9) == unordered_set<StmtNum>{ 10 });

    // Check Parent/Parent*
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6 });
    REQUIRE(PKB::parent->getChildren(6) == unordered_set<StmtNum>{ 7, 8 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(5) == 4);
    REQUIRE(PKB::parent->getParent(7) == 6);
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ 5, 6, 7, 8 });
    REQUIRE(PKB::parent->getChildrenStar(6) == unordered_set<StmtNum>{ 7, 8 });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2, 4, 6 });
    REQUIRE(PKB::uses->getStmtsUses(varID) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1, 2, 4, 5 }); // Container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ 9 });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2, varID1, varID5 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2, 3 }); // Container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{ });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<ID>{ 2, 4, 5 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 4, 6, 7 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 2, 4, 6, 8 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID6) == unordered_set<ID>{ 10 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7, 8 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(9) == unordered_set<ProgLine>{ 10 });

    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(9) == unordered_set<ProgLine>{ 10 });
}

TEST_CASE("Syntactic Errors 1 - Parser-DE Test") {
    vector<string> inputs{
            // Syntax Error
            "procedure star { x = 1 + 2 * a; while (v > 1) { x = 2 if(v==1)then{c = a;}else{ b = 2;}} print p; read s; } ",
            // Invalid SIMPLE
            "abc",
            // Invalid Expression
            "procedure moon { x = (1+2) ** 3; }",
            // Invalid CondExpr
            "procedure moon { if((x+1) > 3 || (a < b)) then {c=a;}else{ b = 2;}}",
            // Missing parenthesis
            "procedure star x = 1 + 2 * a; while (v > 1) { x = 2; if(v==1)then{c = a;}else{ b = 2;}} print p; read s; } ",
            // Missing procName
            "procedure ; { x = 5; }",
            // Invalid constant/variable
            "procedure moon { if(x = a34) then {c=a;}else{ b = 2;}}"
    };

    for (int i = 0; i < inputs.size(); i++) {
        string input = inputs[i];
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        REQUIRE_FALSE(p.parse());
    }
}

TEST_CASE("Syntactic Errors 2 [Naming] - Parser-DE Test") {
    vector<string> inputs{
            // Invalid VarName - digit
            "procedure while { 4head = (1+2) * 3; }",
            // Invalid ProcName - comma
            "procedure , { x = 5; }",
    };

    for (int i = 0; i < inputs.size(); i++) {
        string input = inputs[i];
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        REQUIRE_THROWS(ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs)); // Lexer complains first
    }
}

TEST_CASE("Simple Call - Parser-DE Test") {
    /*
    procedure star {
     1   call call;
      }
    procedure call {
     2   x = a + 1;
    }
     */
    string input = "procedure star { call call; } procedure call { x = a + 1; }";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");

    // Check consts used in AssignStmt
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == false);
    REQUIRE(PKB::constTable->hasConst("3") == false);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == -1); // no Followers

    // Check Uses
    // Stmt 1 `call call` uses `x` in stmt 2 because it proc "call" uses `x` in stmt 2
    REQUIRE(PKB::uses->getStmtsUses(varID1) == unordered_set<StmtNum>{ 1, 2 });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ varID1 });
    REQUIRE(PKB::uses->getVarsUsedByProc(1) == unordered_set<StmtNum>{ varID1 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2 });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(0) == unordered_set<StmtNum>{ varID });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(1) == unordered_set<StmtNum>{ varID });

    // Check ProcTable
    ID procID1 = PKB::procTable->getProcID("star");
    REQUIRE(PKB::procTable->hasProc("star") == true);
    REQUIRE(procID1 == 0);
    ID procID2 = PKB::procTable->getProcID("call");
    REQUIRE(PKB::procTable->hasProc("call") == true);
    REQUIRE(procID2 == 1);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == false);

    // Check Calls
    REQUIRE(PKB::calls->isCalls(0,1));
    REQUIRE(PKB::calls->getCalleeInStmt(1) == 1);

    // p's of Call(p,q)
    REQUIRE(PKB::calls->getCallers(1) == unordered_set<ID>{ 0 });

    // q's of Call(p,q)
    REQUIRE(PKB::calls->getCallees(0) == unordered_set<ID>{ 1 });
}

TEST_CASE("Medium Call - Parser-DE Test") {
    /*
    procedure procA {
     1   call procB;
     }
    procedure procB {
     2   x = a + 1;
     3   if (v > 1) then {
     4       call procC;
         } else {
     5       while (w > 1) {
     6           call procD;
             }
         }
     }
     procedure procC {
     7   y = b + 2;
    }
    procedure procD {
     8   z = c + 3;   
     9   while (t != 5) {
     10       call procE;
         }
    }
    procedure procE {
    11    m = d + 4;
    }
     */
    string input = "procedure procA { call procB; } procedure procB { x = a + 1; if(v > 1) then { call procC; } else { while (w > 1) { call procD; } } } procedure procC { y = b + 2; } procedure procD { z = c + 3; while (t != 5) { call procE; } } procedure procE { m = d + 4; }";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto l = new LexerStub(tok_ptrs);
    auto p = Parser(l);

    REQUIRE(p.parse()); // no errors

    // Check varNames
    ID varID = PKB::varTable->getVarID("x");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "x");
    ID varID1 = PKB::varTable->getVarID("a");
    REQUIRE(varID1 == 1);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID1) == "a");

    // Check consts used in AssignStmt
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->hasConst("3") == true);
    REQUIRE(PKB::constTable->hasConst("4") == true);
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->hasConst("6") == false);

    // Check Uses
    // Stmt 1 `call call` uses `x` in stmt 2 because it proc "call" uses `x` in stmt 2
    ID varID_d = PKB::varTable->getVarID("d");
    REQUIRE(PKB::uses->getStmtsUses(varID_d) == unordered_set<StmtNum>{ 11, 10, 9, 6, 5, 3, 1 });
    ID varID_c = PKB::varTable->getVarID("c");
    REQUIRE(PKB::uses->getStmtsUses(varID_c) == unordered_set<StmtNum>{ 8, 6, 5, 3, 1 });
    ID varID_b = PKB::varTable->getVarID("b");
    REQUIRE(PKB::uses->getStmtsUses(varID_b) == unordered_set<StmtNum>{ 7, 4, 3, 1 });
    ID varID_a = PKB::varTable->getVarID("a");
    REQUIRE(PKB::uses->getStmtsUses(varID_a) == unordered_set<StmtNum>{ 2, 1 });

    ID varID_v = PKB::varTable->getVarID("v");
    ID varID_w = PKB::varTable->getVarID("w");
    ID varID_t = PKB::varTable->getVarID("t");

    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ varID_d, varID_c, varID_b, varID_a, varID_v, varID_w, varID_t });
    REQUIRE(PKB::uses->getVarsUsedByProc(1) == unordered_set<StmtNum>{ varID_d, varID_c, varID_b, varID_a, varID_v, varID_w, varID_t });
    REQUIRE(PKB::uses->getVarsUsedByProc(2) == unordered_set<StmtNum>{ varID_b });
    REQUIRE(PKB::uses->getVarsUsedByProc(3) == unordered_set<StmtNum>{ varID_d, varID_c, varID_t });
    REQUIRE(PKB::uses->getVarsUsedByProc(4) == unordered_set<StmtNum>{ varID_d });

    // Check Modifies
    ID varID_x = PKB::varTable->getVarID("x");
    ID varID_y = PKB::varTable->getVarID("y");
    ID varID_z = PKB::varTable->getVarID("z");
    ID varID_m = PKB::varTable->getVarID("m");
    REQUIRE(PKB::modifies->getStmtsModifies(varID_m) == unordered_set<ID>{ 11, 10, 9, 6, 5, 3, 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID_z) == unordered_set<ID>{ 8, 6, 5, 3, 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID_y) == unordered_set<ID>{ 7, 4, 3, 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID_x) == unordered_set<ID>{ 2, 1 });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(0) == unordered_set<StmtNum>{ varID_x, varID_y, varID_z, varID_m });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(1) == unordered_set<StmtNum>{ varID_x, varID_y, varID_z, varID_m });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(2) == unordered_set<StmtNum>{ varID_y });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(3) == unordered_set<StmtNum>{ varID_m, varID_z });
    REQUIRE(PKB::modifies->getVarsModifiedByProc(4) == unordered_set<StmtNum>{ varID_m });

    // Check ProcTable
    ID procID1 = PKB::procTable->getProcID("procA");
    REQUIRE(PKB::procTable->hasProc("procA") == true);
    REQUIRE(procID1 == 0);
    ID procID2 = PKB::procTable->getProcID("procB");
    REQUIRE(PKB::procTable->hasProc("procB") == true);
    REQUIRE(procID2 == 1);
    ID procID3 = PKB::procTable->getProcID("procC");
    REQUIRE(PKB::procTable->hasProc("procC") == true);
    REQUIRE(procID3 == 2);
    ID procID4 = PKB::procTable->getProcID("procD");
    REQUIRE(PKB::procTable->hasProc("procD") == true);
    REQUIRE(procID4 == 3);
    ID procID5 = PKB::procTable->getProcID("procE");
    REQUIRE(PKB::procTable->hasProc("procE") == true);
    REQUIRE(procID5 == 4);

    // Check StmtLst ??
    REQUIRE(PKB::stmtTable->hasStmt(1) == true);
    REQUIRE(PKB::stmtTable->hasStmt(2) == true);
    REQUIRE(PKB::stmtTable->hasStmt(3) == true);
    REQUIRE(PKB::stmtTable->hasStmt(4) == true);
    REQUIRE(PKB::stmtTable->hasStmt(5) == true);
    REQUIRE(PKB::stmtTable->hasStmt(6) == true);
    REQUIRE(PKB::stmtTable->hasStmt(7) == true);
    REQUIRE(PKB::stmtTable->hasStmt(8) == true);
    REQUIRE(PKB::stmtTable->hasStmt(9) == true);
    REQUIRE(PKB::stmtTable->hasStmt(10) == true);
    REQUIRE(PKB::stmtTable->hasStmt(11) == true);
    REQUIRE(PKB::stmtTable->hasStmt(12) == false);

    // Check Calls
    REQUIRE(PKB::calls->isCalls(0,1));
    REQUIRE(PKB::calls->isCalls(1,2));
    REQUIRE(PKB::calls->isCalls(1,3));
    REQUIRE(PKB::calls->isCalls(3,4));

    REQUIRE(PKB::calls->isCalls(0,0) == false);
    REQUIRE(PKB::calls->isCalls(0,2) == false);
    REQUIRE(PKB::calls->isCalls(0,3) == false);
    REQUIRE(PKB::calls->isCalls(0,4) == false);

    REQUIRE(PKB::calls->isCalls(1,0) == false);
    REQUIRE(PKB::calls->isCalls(1,1) == false);
    REQUIRE(PKB::calls->isCalls(1,4) == false);

    REQUIRE(PKB::calls->isCalls(2,0) == false);
    REQUIRE(PKB::calls->isCalls(2,1) == false);
    REQUIRE(PKB::calls->isCalls(2,2) == false);
    REQUIRE(PKB::calls->isCalls(2,3) == false);
    REQUIRE(PKB::calls->isCalls(2,4) == false);

    REQUIRE(PKB::calls->isCalls(3,0) == false);
    REQUIRE(PKB::calls->isCalls(3,1) == false);
    REQUIRE(PKB::calls->isCalls(3,2) == false);
    REQUIRE(PKB::calls->isCalls(3,3) == false);
    
    REQUIRE(PKB::calls->isCalls(4,0) == false);
    REQUIRE(PKB::calls->isCalls(4,1) == false);
    REQUIRE(PKB::calls->isCalls(4,2) == false);
    REQUIRE(PKB::calls->isCalls(4,3) == false);
    REQUIRE(PKB::calls->isCalls(4,4) == false);

    REQUIRE(PKB::calls->getCalleeInStmt(1) == 1);
    REQUIRE(PKB::calls->getCalleeInStmt(4) == 2);
    REQUIRE(PKB::calls->getCalleeInStmt(6) == 3);
    REQUIRE(PKB::calls->getCalleeInStmt(10) == 4);

    // p's of Call(p,q)
    REQUIRE(PKB::calls->getCallers(1) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::calls->getCallers(2) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::calls->getCallers(3) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::calls->getCallers(4) == unordered_set<ID>{ 3 });

    // q's of Call(p,q)
    REQUIRE(PKB::calls->getCallees(0) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::calls->getCallees(1) == unordered_set<ID>{ 2, 3 });
    REQUIRE(PKB::calls->getCallees(3) == unordered_set<ID>{ 4 });

    // p's of Call*(p,q)
    REQUIRE(PKB::calls->getCallersStar(1) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::calls->getCallersStar(2) == unordered_set<ID>{ 1, 0 });
    REQUIRE(PKB::calls->getCallersStar(3) == unordered_set<ID>{ 1, 0 });
    REQUIRE(PKB::calls->getCallersStar(4) == unordered_set<ID>{ 3, 1, 0 });

    // q's of Call*(p,q)
    REQUIRE(PKB::calls->getCalleesStar(0) == unordered_set<ID>{ 1, 2, 3, 4 });
    REQUIRE(PKB::calls->getCalleesStar(1) == unordered_set<ID>{ 2, 3, 4 });
    REQUIRE(PKB::calls->getCalleesStar(3) == unordered_set<ID>{ 4 });
}