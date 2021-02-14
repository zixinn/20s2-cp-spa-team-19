#include "catch.hpp"

#include "AST/Index.h"
#include "SP/Parser.h"
#include "PKB/PKB.h"

using namespace std;

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
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{  });
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
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{  });
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, varID1, varID4 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2, 3 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<ID>{ 2, 4, 5 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 4, 6 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID5) == unordered_set<ID>{ 8 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<ID>{  });

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
    REQUIRE(PKB::follows->getFollower(2) != 57);
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
}

TEST_CASE("Triple-nested if test - Parser-DE Test") {
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
    REQUIRE(PKB::follows->getFollower(2) != 57);
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