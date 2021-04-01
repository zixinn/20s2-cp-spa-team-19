#include "SP/DesignExtractor.h"
#include "PKB/PKB.h"
#include "AST/Index.h"
#include "SP/Parser.h"

#include "catch.hpp"
using namespace std;

// Uses Parser directly
// Utility functions
PrintStmt* createPrint(STRING varName) {
    std::vector<sp::Token*> printTokens{
            // Print Stmt
            new sp::Token(sp::Token::TokenType::PRINT, "print"),
            new sp::Token(sp::Token::TokenType::NAME, varName),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto lprint = new LexerStub(printTokens);

    Parser pprint = Parser(lprint);
    ast::PrintStmt* printStmt = pprint.parsePrintStmt();
    return printStmt;
}

ReadStmt* createRead(STRING varName) {
    std::vector<sp::Token*> ReadTokens{
            // Read Stmt
            new sp::Token(sp::Token::TokenType::READ, "read"),
            new sp::Token(sp::Token::TokenType::NAME, "varName"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto lread = new LexerStub(ReadTokens);
    Parser pread = Parser(lread);
    ast::ReadStmt* readStmt = pread.parseReadStmt();
    return readStmt;
}

TEST_CASE("ONE PROCEDURE - storeNewProcedure and exitProcedure Test (no While/Ifs)") {
    DesignExtractor::signalReset();

    std::vector<sp::Token*> stubAssignTokens{
            // Assignment Stmt
            new sp::Token(sp::Token::TokenType::NAME, "scaramouche"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "tartaglia"),
            new sp::Token(sp::Token::TokenType::PLUS, "+"),
            new sp::Token(sp::Token::TokenType::NAME, "pustota"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "2"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "0"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::EOFF, "EOF")
    };


    auto lass = new LexerStub(stubAssignTokens);
    Parser pass = Parser(lass);
    ast::AssignStmt* assStmt = pass.parseAssignStmt();

    DesignExtractor::storeNewProcedure("strobelight");

    DesignExtractor::storeNewPrint(1, "reason", createPrint("reason"));
    DesignExtractor::storeNewRead(2, "dyed", createRead("dyed"));
    DesignExtractor::storeNewAssignment(3, "scaramouche", assStmt);

    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check procTable
    ID procID = PKB::procTable->getProcID("strobelight");
    REQUIRE(PKB::procTable->hasProc("strobelight") == true);
    REQUIRE(procID == 0);
    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);

    // Check varNames
    ID varID = PKB::varTable->getVarID("reason");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "reason");
    ID varID1 = PKB::varTable->getVarID("dyed");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "dyed");
    ID varID2 = PKB::varTable->getVarID("scaramouche");
    REQUIRE(varID2 == 2);
    REQUIRE(PKB::varTable->getVarName(varID2) == "scaramouche");
    ID varID3 = PKB::varTable->getVarID("tartaglia");
    REQUIRE(varID3 == 3);
    REQUIRE(PKB::varTable->getVarName(varID3) == "tartaglia");
    ID varID4 = PKB::varTable->getVarID("pustota");
    REQUIRE(varID4 == 4);
    REQUIRE(PKB::varTable->getVarName(varID4) == "pustota");

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) == 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);
    REQUIRE(PKB::follows->getFollower(0) == -1);
    REQUIRE(PKB::follows->getFollower(10) == -1);
    // Check Follows*
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<ID> { 2, 3 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<ID> { 3 });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<ID> { });

    // Check Modifies
    // Assign Stmt
    REQUIRE(PKB::modifies->getProcsModifies(varID2) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<StmtNum>{ 3 });
    // Read Stmt
    REQUIRE(PKB::modifies->getProcsModifies(varID1) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<StmtNum>{ 2 });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varID) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(varID3) == unordered_set<StmtNum>{ 3 });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ 3 });
    REQUIRE(PKB::uses->getVarsUsedByProc(procID) == unordered_set<StmtNum>{ varID, varID3, varID4 });
}

TEST_CASE("storeNewAssignment Const RHS Test") {
    DesignExtractor::signalReset();
    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::CONST, "420"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    DesignExtractor::storeNewAssignment(1, "axel", assignment);
    // Check varName
    ID varID = PKB::varTable->getVarID("axel");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel");
    // Check ConstTable
    REQUIRE(PKB::constTable->hasConst("420") == true);
    REQUIRE(PKB::constTable->getConstValue("420") == 420);
    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
    // No Uses since RHS is a constant
}

TEST_CASE("storeNewAssignment VarName RHS Test") {
    DesignExtractor::signalReset();
    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    // Check varName, LHS
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");

    // Check varName, RHS
    ID varIDRHS = PKB::varTable->getVarID("semelparity");
    REQUIRE(varIDRHS == 1);    // second variable in varTable
    REQUIRE(PKB::varTable->getVarName(varIDRHS) == "semelparity");

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varIDRHS) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
}

TEST_CASE("storeNewAssignment InfixExpr containing VarNames and Consts Test") {
    DesignExtractor::signalReset();
    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "eternal"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "senescence"),
            new sp::Token(sp::Token::TokenType::PLUS, "+"),
            new sp::Token(sp::Token::TokenType::NAME, "sequestration"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "2"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "0"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::EOFF, "EOF"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    DesignExtractor::storeNewAssignment(1, "eternal", assignment);
    // Check varName, LHS
    ID varID = PKB::varTable->getVarID("eternal");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "eternal");

    // Check varName, RHS
    ID varIDRHS = PKB::varTable->getVarID("senescence");
    REQUIRE(varIDRHS == 1);    // second variable in varTable
    REQUIRE(PKB::varTable->getVarName(varIDRHS) == "senescence");
    ID varIDRHS2 = PKB::varTable->getVarID("sequestration");
    REQUIRE(varIDRHS2 == 2);    // second variable in varTable
    REQUIRE(PKB::varTable->getVarName(varIDRHS2) == "sequestration");

    // Check ConstTable
    REQUIRE(PKB::constTable->hasConst("2") == true);
    REQUIRE(PKB::constTable->getConstValue("2") == 2);
    REQUIRE(PKB::constTable->hasConst("0") == true);
    REQUIRE(PKB::constTable->getConstValue("0") == 0);

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varIDRHS) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
    REQUIRE(PKB::uses->getStmtsUses(varIDRHS2) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
}

TEST_CASE("storeNewRead Test") {
    DesignExtractor::signalReset();

    DesignExtractor::storeNewRead(1, "procedure", createRead("procedure"));

    // Check varName
    ID varID = PKB::varTable->getVarID("procedure");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "procedure");

    // Check readVariables
    REQUIRE(PKB::stmtTable->getReadVariableOfStmt(1) == varID);
    REQUIRE(PKB::stmtTable->getStmtNumsOfReadWithVar(varID) == unordered_set<StmtNum>({1}));

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
}

TEST_CASE("storeNewPrint Test") {
    DesignExtractor::signalReset();

    DesignExtractor::storeNewPrint(1, "defenestration", createPrint("defenestration"));

    // Check varName
    ID varID = PKB::varTable->getVarID("defenestration");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "defenestration");

    // Check printVariables
    REQUIRE(PKB::stmtTable->getPrintVariableOfStmt(1) == varID);
    REQUIRE(PKB::stmtTable->getStmtNumsOfPrintWithVar(varID) == unordered_set<StmtNum>({1}));

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varID) == unordered_set<StmtNum>{ 1 }); // StmtNums start from 1
}

TEST_CASE("[SIMPLE, no nested while/if] storeNewWhile and exitWhile Test") {
    DesignExtractor::signalReset();
    // Create While AST
    // Note that the while loop's internal is empty, but the test itself adds statements inside this while loop
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "while ((x == 5) || (y > 10)) { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lwhile = new LexerStub(tok_ptrs);
    auto pwhile = Parser(lwhile);
    ast::WhileStmt* whileStmt = pwhile.parseWhileStmt();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of while loop
    vector<STRING> condConsts{ "5", "10" };
    DesignExtractor::storeNewProcedure("hana");
    DesignExtractor::storeNewWhile(1,condVarNames, condConsts, whileStmt);
    DesignExtractor::storeNewAssignment(2, "axel2", assignment);
    DesignExtractor::storeNewRead(3, "shine", createRead("shine"));
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == true); // StmtLst of while loop
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == false); // Stmt 3 not in a separate stmtLst
    // Check varNames
    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 1);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");
    ID varID3 = PKB::varTable->getVarID("shine");
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "shine");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 1 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("0") == false);

    // Check Follows (in while stmtlst)
    REQUIRE(PKB::follows->getFollower(1) != 2); // 2 does not follow 1 because 2 is nested in 1
    REQUIRE(PKB::follows->getFollower(2) == 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);

    // Check Parent
    REQUIRE(PKB::parent->getChildren(1) == unordered_set<StmtNum>{ 2, 3 });
    REQUIRE(PKB::parent->getParent(2) == 1);
    REQUIRE(PKB::parent->getParent(3) == 1);
    REQUIRE(PKB::parent->getParent(1) == -1);   // No parent

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2 }); // as 1 is a container stmt

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2 }); // as 1 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 1, 3 }); // as 1 is a container stmt
    REQUIRE(PKB::modifies->getProcsModifies(varID3) == unordered_set<ID>{ 0 });
}

TEST_CASE("[ONE NESTED WHILE] storeNewWhile and exitWhile Test") {
    DesignExtractor::signalReset();
    // Create While AST
    // Note that the while loop's internal is empty, but the test itself adds statements inside this while loop
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "while ((x == 5) || (y > 10)) { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lwhile = new LexerStub(tok_ptrs);
    auto pwhile = Parser(lwhile);
    ast::WhileStmt* whileStmt = pwhile.parseWhileStmt();

    STRING input2 = "while ((a != 33) || (z > 1)) { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedWhile = new LexerStub(tok_ptrs2);
    auto pNestedWhile = Parser(lNestedWhile);
    ast::WhileStmt* nestedWhileStmt = pNestedWhile.parseWhileStmt();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of while loop
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> nestedCondVarNames{ "a", "z" };    // Used in conditional expression of nested while loop
    vector<STRING> nestedCondConsts{ "33", "1" };
    DesignExtractor::storeNewProcedure("hana");
    DesignExtractor::storeNewWhile(1,condVarNames, condConsts, whileStmt);
    DesignExtractor::storeNewAssignment(2, "axel2", assignment);
    DesignExtractor::storeNewWhile(3,nestedCondVarNames, nestedCondConsts, nestedWhileStmt);
    DesignExtractor::storeNewRead(4, "shine", createRead("shine"));
    DesignExtractor::exitWhile();
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // StmtLst of procedure
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == true); // StmtLst of first while loop
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == false); // Line containing nested while loop
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // Line containing nested while loop's stmtLst

    // Check cond varNames
    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 1);
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");
    ID condVarId3 = PKB::varTable->getVarID("a");
    REQUIRE(condVarId3 == 4);   // Gap in varID because of line 2 AssignStmt
    REQUIRE(PKB::varTable->getVarName(condVarId3) == "a");
    ID condVarId4 = PKB::varTable->getVarID("z");
    REQUIRE(condVarId4 == 5);
    REQUIRE(PKB::varTable->getVarName(condVarId4) == "z");
    // Check rest ofvarNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 3);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");
    ID varID3 = PKB::varTable->getVarID("shine");
    REQUIRE(varID3 == 6);   // due to cond variables in nested While loop
    REQUIRE(PKB::varTable->getVarName(varID3) == "shine");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId3) == unordered_set<StmtNum>{ 3 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId4) == unordered_set<StmtNum>{ 3 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("0") == false);
    REQUIRE(PKB::constTable->hasConst("33") == true);
    REQUIRE(PKB::constTable->getConstValue("33") == 33);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->getConstValue("1") == 1);

    // Check Follows (in while stmtlst)
    REQUIRE(PKB::follows->getFollower(1) != 2);
    REQUIRE(PKB::follows->getFollower(2) == 3); // nested While follows stmt 2
    REQUIRE(PKB::follows->getFollower(3) != 4);
    REQUIRE(PKB::follows->getFollower(1) != 3); // nested While does not follow parent While

    // Check Parent
    REQUIRE(PKB::parent->getChildren(1) == unordered_set<StmtNum>{ 2, 3 });
    REQUIRE(PKB::parent->getChildren(3) == unordered_set<StmtNum>{ 4 });
    REQUIRE(PKB::parent->getParent(2) == 1);
    REQUIRE(PKB::parent->getParent(3) == 1);
    REQUIRE(PKB::parent->getParent(4) == 3);

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId3) == unordered_set<StmtNum>{ 1, 3 }); // Nested while
    REQUIRE(PKB::uses->getStmtsUses(condVarId4) == unordered_set<StmtNum>{ 1, 3 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2 }); // as 1 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2,
                                                                       condVarId3, condVarId4, varID2 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1, 2 }); // as 1 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 1, 3, 4 }); // as 1, 3 are container stmts
    REQUIRE(PKB::modifies->getProcsModifies(varID3) == unordered_set<ID>{ 0 });
}

TEST_CASE("[SIMPLE, no nested if/while] storeNewIf and storeNewElse and endIfElse Test") {
    DesignExtractor::signalReset();
    // Create If ASTs
    // Note that the if stmt's internal is empty, but the test itself adds statements inside this if stmt
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "if ((x == 5) || (y > 10)) then { x = 1; } else { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    std::vector<sp::Token*> stub2Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "slalom"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l2 = new LexerStub(stub2Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p2 = Parser(l2);
    ast::AssignStmt* assignment2 = p2.parseAssignStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    DesignExtractor::storeNewProcedure("kanzashi");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment2);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewRead(4, "shine", createRead("shine"));
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // If stmt itself
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // If stmt's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // Else's stmtLst

    // Check varNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 1);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");

    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 3);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");

    ID varID3 = PKB::varTable->getVarID("slalom");  // Both Assignments' exprs are the same, so no new var added.
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "slalom");

    ID varID4 = PKB::varTable->getVarID("shine");
    REQUIRE(varID4 == 5);
    REQUIRE(PKB::varTable->getVarName(varID4) == "shine");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 2 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("0") == false);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); // 3 is nested in 2
    REQUIRE(PKB::follows->getFollower(2) != 4); // 4 nested in 2
    REQUIRE(PKB::follows->getFollower(3) != 4); // 4 in separate stmtLst

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(4) == 2);

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2, varID2 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 2, 4 }); // as 2 is a container stmt

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));
}

TEST_CASE("[ONE NESTED IF] storeNewIf and storeNewElse and endIfElse Test") {
    DesignExtractor::signalReset();

    // Create If ASTs
    // Note that the if stmt's internal is empty, but the test itself adds statements inside this if stmt
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "if ((x == 5) || (y > 10)) then { x = 1; } else { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    STRING input2 = "if ((a != 33) || (z > 1)) then { x = 1; } else { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedIf = new LexerStub(tok_ptrs2);
    auto pNestedIf = Parser(lNestedIf);
    ast::IfStmt* nestedIfStmt = pNestedIf.parseIfStmt();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    std::vector<sp::Token*> stub2Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "slalom"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l2 = new LexerStub(stub2Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p2 = Parser(l2);
    ast::AssignStmt* assignment2 = p2.parseAssignStmt();

    std::vector<sp::Token*> stub3Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "quartz"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "conspiracy"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l3 = new LexerStub(stub3Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p3 = Parser(l3);
    ast::AssignStmt* assignment3 = p3.parseAssignStmt();

    std::vector<sp::Token*> stub4Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "sapphire"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::CONST, "100"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l4 = new LexerStub(stub4Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p4 = Parser(l4);
    ast::AssignStmt* assignment4 = p4.parseAssignStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> nestedCondVarNames{ "a", "z" };    // Used in conditional expression of nested if stmt
    vector<STRING> nestedCondConsts{ "33", "1" };
    DesignExtractor::storeNewProcedure("mitosis");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment2);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewIf(4,nestedCondVarNames, nestedCondConsts, nestedIfStmt);    // Nested If-then
    DesignExtractor::storeNewAssignment(5, "quartz", assignment3);
    DesignExtractor::storeNewElse();                                                    // Nested Else
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment4);
    DesignExtractor::storeNewRead(7, "droning", createRead("droning"));
    DesignExtractor::endIfElse();
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // If stmt itself
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // If stmt's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // First Else's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // Nested If's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true); // Nested Else's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 1);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");

    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 3);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");

    ID varID3 = PKB::varTable->getVarID("slalom");  // Both Assignments' exprs are the same, so no new var added.
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "slalom");

    // Nested If vars
    ID condVarId3 = PKB::varTable->getVarID("a");
    REQUIRE(condVarId3 == 5);   // Gap in varID because of line 2 AssignStmt
    REQUIRE(PKB::varTable->getVarName(condVarId3) == "a");
    ID condVarId4 = PKB::varTable->getVarID("z");
    REQUIRE(condVarId4 == 6);
    REQUIRE(PKB::varTable->getVarName(condVarId4) == "z");

    ID varID4 = PKB::varTable->getVarID("quartz");
    REQUIRE(varID4 == 7);
    REQUIRE(PKB::varTable->getVarName(varID4) == "quartz");
    ID varID5 = PKB::varTable->getVarID("conspiracy");
    REQUIRE(varID5 == 8);
    REQUIRE(PKB::varTable->getVarName(varID5) == "conspiracy");
    ID varID6 = PKB::varTable->getVarID("sapphire");
    REQUIRE(varID6 == 9);
    REQUIRE(PKB::varTable->getVarName(varID6) == "sapphire");
    ID varID7 = PKB::varTable->getVarID("droning");
    REQUIRE(varID7 == 10);
    REQUIRE(PKB::varTable->getVarName(varID7) == "droning");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId3) == unordered_set<StmtNum>{ 4 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId4) == unordered_set<StmtNum>{ 4 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("-50") == false);
    REQUIRE(PKB::constTable->hasConst("33") == true);
    REQUIRE(PKB::constTable->getConstValue("33") == 33);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->getConstValue("1") == 1);
    REQUIRE(PKB::constTable->hasConst("100") == true);
    REQUIRE(PKB::constTable->getConstValue("100") == 100);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); // 3 is nested in 2
    REQUIRE(PKB::follows->getFollower(2) != 4); // 4 nested in 2
    REQUIRE(PKB::follows->getFollower(3) != 4); // 4 in separate stmtLst
    REQUIRE(PKB::follows->getFollower(4) != 5); // 5 is nested in 4
    REQUIRE(PKB::follows->getFollower(4) != 6); // 6 is nested in 4
    REQUIRE(PKB::follows->getFollower(5) != 6); // different stmtLsts
    REQUIRE(PKB::follows->getFollower(6) != 6); // stmt cannot follow itself
    REQUIRE(PKB::follows->getFollower(6) == 7);

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6, 7 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(7) == 4);
    REQUIRE(PKB::parent->getParent(1) == -1);

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId3) == unordered_set<StmtNum>{ 2, 4 }); // Nested if
    REQUIRE(PKB::uses->getStmtsUses(condVarId4) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ 2, 4, 5 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2,
                                                                       condVarId3, condVarId4, varID2, varID5 });
    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 2, 4, 5 }); // 2, 4 is container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID6) == unordered_set<ID>{ 2, 4, 6 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID7) == unordered_set<ID>{ 2, 4, 7 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{5, 7}));
    REQUIRE(PKB::stmtTable->getIfStmtRange(4) == make_pair(5, set<int>{5}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(4) == make_pair(6, set<int>{7}));
}

TEST_CASE("[WHILE-IF NESTING] storeNewWhile & storeNewIf Interaction Test") {
    DesignExtractor::signalReset();

    // Create If AST
    // Note that the if stmt's internal is empty, but the test itself adds statements inside this if stmt
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "if ((x == 5) || (y > 10)) then { x = 1; } else { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    STRING input2 = "while ((a != 33) || (z > 1)) { x = 1; }"; //x = 1 is dummy statement
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedWhile = new LexerStub(tok_ptrs2);
    auto pNestedWhile = Parser(lNestedWhile);
    ast::WhileStmt* nestedWhileStmt = pNestedWhile.parseWhileStmt();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    std::vector<sp::Token*> stub2Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "slalom"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l2 = new LexerStub(stub2Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p2 = Parser(l2);
    ast::AssignStmt* assignment2 = p2.parseAssignStmt();

    std::vector<sp::Token*> stub3Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "quartz"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "conspiracy"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l3 = new LexerStub(stub3Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p3 = Parser(l3);
    ast::AssignStmt* assignment3 = p3.parseAssignStmt();

    std::vector<sp::Token*> stub4Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "sapphire"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::CONST, "100"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l4 = new LexerStub(stub4Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p4 = Parser(l4);
    ast::AssignStmt* assignment4 = p4.parseAssignStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> nestedCondVarNames{ "a", "z" };    // Used in conditional expression of nested if stmt
    vector<STRING> nestedCondConsts{ "33", "1" };
    DesignExtractor::storeNewProcedure("arabesque");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment2);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewWhile(4,nestedCondVarNames, nestedCondConsts, nestedWhileStmt);    // Nested While
    DesignExtractor::storeNewAssignment(5, "quartz", assignment3);
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment4);
    DesignExtractor::storeNewRead(7, "droning", createRead("droning"));
    DesignExtractor::exitWhile();
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // If stmt itself
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // If stmt's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // First Else's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // Nested While's's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 1);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");

    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 3);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");

    ID varID3 = PKB::varTable->getVarID("slalom");  // Both Assignments' exprs are the same, so no new var added.
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "slalom");

    // Nested If vars
    ID condVarId3 = PKB::varTable->getVarID("a");
    REQUIRE(condVarId3 == 5);   // Gap in varID because of line 2 AssignStmt
    REQUIRE(PKB::varTable->getVarName(condVarId3) == "a");
    ID condVarId4 = PKB::varTable->getVarID("z");
    REQUIRE(condVarId4 == 6);
    REQUIRE(PKB::varTable->getVarName(condVarId4) == "z");

    ID varID4 = PKB::varTable->getVarID("quartz");
    REQUIRE(varID4 == 7);
    REQUIRE(PKB::varTable->getVarName(varID4) == "quartz");
    ID varID5 = PKB::varTable->getVarID("conspiracy");
    REQUIRE(varID5 == 8);
    REQUIRE(PKB::varTable->getVarName(varID5) == "conspiracy");
    ID varID6 = PKB::varTable->getVarID("sapphire");
    REQUIRE(varID6 == 9);
    REQUIRE(PKB::varTable->getVarName(varID6) == "sapphire");
    ID varID7 = PKB::varTable->getVarID("droning");
    REQUIRE(varID7 == 10);
    REQUIRE(PKB::varTable->getVarName(varID7) == "droning");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId3) == unordered_set<StmtNum>{ 4 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId4) == unordered_set<StmtNum>{ 4 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("-50") == false);
    REQUIRE(PKB::constTable->hasConst("33") == true);
    REQUIRE(PKB::constTable->getConstValue("33") == 33);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->getConstValue("1") == 1);
    REQUIRE(PKB::constTable->hasConst("100") == true);
    REQUIRE(PKB::constTable->getConstValue("100") == 100);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); // 3 is nested in 2
    REQUIRE(PKB::follows->getFollower(2) != 4); // 4 nested in 2
    REQUIRE(PKB::follows->getFollower(3) != 4); // 4 in separate stmtLst
    REQUIRE(PKB::follows->getFollower(4) != 5); // 5 is nested in 4
    REQUIRE(PKB::follows->getFollower(4) != 6); // 6 is nested in 4
    REQUIRE(PKB::follows->getFollower(5) == 6); // nested in while loop
    REQUIRE(PKB::follows->getFollower(6) == 7);
    REQUIRE(PKB::follows->getFollower(6) != 6); // stmt cannot follow itself
    // Check Follows*
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<ID> { 2 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<ID> { 6, 7 });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<ID> { 7 });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<ID> { });

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 });
    REQUIRE(PKB::parent->getChildren(4) == unordered_set<StmtNum>{ 5, 6, 7 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(7) == 4);
    REQUIRE(PKB::parent->getParent(1) == -1);
    // Check Parent*
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ 5, 6, 7 });
    REQUIRE(PKB::parent->getChildrenStar(5) == unordered_set<StmtNum>{ });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId3) == unordered_set<StmtNum>{ 2, 4 }); // Nested if
    REQUIRE(PKB::uses->getStmtsUses(condVarId4) == unordered_set<StmtNum>{ 2, 4 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ 2, 4, 5 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2,
                                                                       condVarId3, condVarId4, varID2, varID5 });
    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 3 }); // as 2 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 2, 4, 5 }); // 2, 4 is container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID6) == unordered_set<ID>{ 2, 4, 6 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID7) == unordered_set<ID>{ 2, 4, 7 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 4, 5, 6, 7 });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));
}

TEST_CASE("Multi-procedure test") {
    DesignExtractor::signalReset();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    std::vector<sp::Token*> stub2Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "slalom"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l2 = new LexerStub(stub2Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p2 = Parser(l2);
    ast::AssignStmt* assignment2 = p2.parseAssignStmt();

    std::vector<sp::Token*> stub3Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "quartz"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "conspiracy"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l3 = new LexerStub(stub3Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p3 = Parser(l3);
    ast::AssignStmt* assignment3 = p3.parseAssignStmt();

    std::vector<sp::Token*> stub4Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "sapphire"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::CONST, "100"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l4 = new LexerStub(stub4Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p4 = Parser(l4);
    ast::AssignStmt* assignment4 = p4.parseAssignStmt();

    // Create If AST
    // Note that the if stmt's internal is empty, but the test itself adds statements inside this if stmt
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "if ((x == 5) || (y > 10)) then { x = x + 1; } else { x = x + 1; }";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    STRING input2 = "while ((a != 33) || (z > 1)) { x = x + 1; }";
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedWhile = new LexerStub(tok_ptrs2);
    auto pNestedWhile = Parser(lNestedWhile);
    ast::WhileStmt* nestedWhileStmt = pNestedWhile.parseWhileStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> whileCondVarNames{ "a", "z" };    // Used in conditional expression of nested if stmt
    vector<STRING> whileCondConsts{ "33", "1" };

    DesignExtractor::storeNewProcedure("arabesque");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment2);
    DesignExtractor::storeNewElse();
    DesignExtractor::storeNewAssignment(4, "slalom", assignment2);
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();

    DesignExtractor::storeNewProcedure("deux");
    DesignExtractor::storeNewWhile(5, whileCondVarNames, whileCondConsts, nestedWhileStmt);
    DesignExtractor::storeNewAssignment(6, "quartz", assignment3);
    DesignExtractor::storeNewAssignment(7, "sapphire", assignment4);
    DesignExtractor::storeNewRead(8,"droning", createRead("droning"));
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check procTable
    ID procID = PKB::procTable->getProcID("arabesque");
    REQUIRE(PKB::procTable->hasProc("arabesque") == true);
    REQUIRE(procID == 0);
    ID procID2 = PKB::procTable->getProcID("deux");
    REQUIRE(PKB::procTable->hasProc("deux") == true);
    REQUIRE(procID2 == 1);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // If stmt itself
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // If stmt's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // Else's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true); // Nested While's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 1);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");

    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 3);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");

    ID varID3 = PKB::varTable->getVarID("slalom");  // Both Assignments' exprs are the same, so no new var added.
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "slalom");

    // Nested If vars
    ID condVarId3 = PKB::varTable->getVarID("a");
    REQUIRE(condVarId3 == 5);   // Gap in varID because of line 2 AssignStmt
    REQUIRE(PKB::varTable->getVarName(condVarId3) == "a");
    ID condVarId4 = PKB::varTable->getVarID("z");
    REQUIRE(condVarId4 == 6);
    REQUIRE(PKB::varTable->getVarName(condVarId4) == "z");

    ID varID4 = PKB::varTable->getVarID("quartz");
    REQUIRE(varID4 == 7);
    REQUIRE(PKB::varTable->getVarName(varID4) == "quartz");
    ID varID5 = PKB::varTable->getVarID("conspiracy");
    REQUIRE(varID5 == 8);
    REQUIRE(PKB::varTable->getVarName(varID5) == "conspiracy");
    ID varID6 = PKB::varTable->getVarID("sapphire");
    REQUIRE(varID6 == 9);
    REQUIRE(PKB::varTable->getVarName(varID6) == "sapphire");
    ID varID7 = PKB::varTable->getVarID("droning");
    REQUIRE(varID7 == 10);
    REQUIRE(PKB::varTable->getVarName(varID7) == "droning");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId3) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId4) == unordered_set<StmtNum>{ 5 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("-50") == false);
    REQUIRE(PKB::constTable->hasConst("33") == true);
    REQUIRE(PKB::constTable->getConstValue("33") == 33);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->getConstValue("1") == 1);
    REQUIRE(PKB::constTable->hasConst("100") == true);
    REQUIRE(PKB::constTable->getConstValue("100") == 100);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); // 3 is nested in 2
    REQUIRE(PKB::follows->getFollower(2) != 4); // 4 nested in 2
    REQUIRE(PKB::follows->getFollower(3) != 4); // 4 in else
    REQUIRE(PKB::follows->getFollower(4) != 5); // 5 in another procedure
    REQUIRE(PKB::follows->getFollower(5) != 6); // 6 is nested in 5
    REQUIRE(PKB::follows->getFollower(5) != 7); // 7 is nested in 5
    REQUIRE(PKB::follows->getFollower(5) != 8); // 8 is nested in 5
    REQUIRE(PKB::follows->getFollower(6) == 7); // nested in while loop
    REQUIRE(PKB::follows->getFollower(7) == 8); // nested in while loop
    REQUIRE(PKB::follows->getFollower(7) != 7); // stmt cannot follow itself
    // Check Follows*
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<ID> { 2 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<ID> { 7, 8 });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<ID> { 8 });

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(5) == unordered_set<StmtNum>{ 6, 7, 8 });
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(4) == 2);
    REQUIRE(PKB::parent->getParent(6) == 5);
    REQUIRE(PKB::parent->getParent(7) == 5);
    REQUIRE(PKB::parent->getParent(8) == 5);
    REQUIRE(PKB::parent->getParent(1) == -1);
    // Check Parent*
    REQUIRE(PKB::parent->getChildrenStar(1) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4 });
    REQUIRE(PKB::parent->getChildrenStar(3) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(5) == unordered_set<StmtNum>{ 6, 7, 8 });
    REQUIRE(PKB::parent->getChildrenStar(6) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(7) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(8) == unordered_set<StmtNum>{ });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId3) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId4) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2, 3, 4 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ 5, 6 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2, varID2 });
    REQUIRE(PKB::uses->getVarsUsedByProc(1) == unordered_set<StmtNum>{ condVarId3, condVarId4, varID5 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 3, 4 }); // as 2 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 5, 6 }); // as 4 is container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID6) == unordered_set<ID>{ 5, 7 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID7) == unordered_set<ID>{ 5, 8 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });   // Next only holds for stmts in the same procedure
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ });   // Next only holds for stmts in the same procedure
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 6 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 5 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 5, 6, 7, 8 });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));
}

TEST_CASE("Multi-procedure test + calls") {
    DesignExtractor::signalReset();

    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    std::vector<sp::Token*> stub2Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "slalom"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l2 = new LexerStub(stub2Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p2 = Parser(l2);
    ast::AssignStmt* assignment2 = p2.parseAssignStmt();

    std::vector<sp::Token*> stub3Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "quartz"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "conspiracy"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l3 = new LexerStub(stub3Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p3 = Parser(l3);
    ast::AssignStmt* assignment3 = p3.parseAssignStmt();

    std::vector<sp::Token*> stub4Tokens{
            new sp::Token(sp::Token::TokenType::NAME, "sapphire"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::CONST, "100"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l4 = new LexerStub(stub4Tokens);     //new keyword gets me a ptr to LexerStub
    Parser p4 = Parser(l4);
    ast::AssignStmt* assignment4 = p4.parseAssignStmt();

    // Create If AST
    // Note that the if stmt's internal is empty, but the test itself adds statements inside this if stmt
    // This is because in this case, the AST itself has no bearing on the DE's actions (it is only stored into the PKB)
    STRING input = "if ((x == 5) || (y > 10)) then { x = x + 1; } else { x = x + 1; }";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    STRING input2 = "while ((a != 33) || (z > 1)) { x = x + 1; }";
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedWhile = new LexerStub(tok_ptrs2);
    auto pNestedWhile = Parser(lNestedWhile);
    ast::WhileStmt* nestedWhileStmt = pNestedWhile.parseWhileStmt();

    // Create call AST
    STRING input3 = "call abyssal;";
    std::vector<sp::Token> actual_tok3;
    std::vector<sp::Token*> tok_ptrs3;
    ParserUtils::StringToTokenPtrs(input3, actual_tok3, tok_ptrs3);
    auto lCall = new LexerStub(tok_ptrs3);
    auto pCall = Parser(lCall);
    ast::CallStmt* callStmt = pCall.parseCallStmt();

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> whileCondVarNames{ "a", "z" };    // Used in conditional expression of nested if stmt
    vector<STRING> whileCondConsts{ "33", "1" };

    DesignExtractor::storeNewProcedure("arabesque");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment2);
    DesignExtractor::storeNewElse();
    DesignExtractor::storeNewAssignment(4, "slalom", assignment2);
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();

    DesignExtractor::storeNewProcedure("deux");
    DesignExtractor::storeNewWhile(5,whileCondVarNames, whileCondConsts, nestedWhileStmt);
    DesignExtractor::storeNewAssignment(6, "quartz", assignment3);
    DesignExtractor::storeNewAssignment(7, "sapphire", assignment4);
    DesignExtractor::storeNewRead(8, "droning", createRead("droning"));
    REQUIRE(DesignExtractor::storeNewCall(9, "deux", "abyssal", callStmt) == true);
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();

    DesignExtractor::storeNewProcedure("abyssal");
    DesignExtractor::storeNewRead(10, "IA", createRead("IA"));
    DesignExtractor::storeNewPrint(11, "chalk", createPrint("chalk"));
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check procTable
    ID procID = PKB::procTable->getProcID("arabesque");
    REQUIRE(PKB::procTable->hasProc("arabesque") == true);
    REQUIRE(procID == 0);
    ID procID2 = PKB::procTable->getProcID("deux");
    REQUIRE(PKB::procTable->hasProc("deux") == true);
    REQUIRE(procID2 == 1);
    ID procID3 = PKB::procTable->getProcID("abyssal");
    REQUIRE(PKB::procTable->hasProc("abyssal") == true);
    REQUIRE(procID3 == 2);

    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);  // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(2) == false); // If stmt itself
    REQUIRE(PKB::stmtLstTable->hasStmtLst(3) == true); // If stmt's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(4) == true); // Else's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(5) == true); // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(6) == true); // Nested While's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(7) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(8) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(9) == false);
    REQUIRE(PKB::stmtLstTable->hasStmtLst(10) == true); // Procedure's stmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(11) == false);

    // Check varNames
    ID varID = PKB::varTable->getVarID("axel2");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "axel2");
    ID varID2 = PKB::varTable->getVarID("semelparity");
    REQUIRE(varID2 == 1);
    REQUIRE(PKB::varTable->getVarName(varID2) == "semelparity");

    ID condVarId = PKB::varTable->getVarID("x");
    REQUIRE(condVarId == 2);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId) == "x");
    ID condVarId2 = PKB::varTable->getVarID("y");
    REQUIRE(condVarId2 == 3);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(condVarId2) == "y");

    ID varID3 = PKB::varTable->getVarID("slalom");  // Both Assignments' exprs are the same, so no new var added.
    REQUIRE(varID3 == 4);
    REQUIRE(PKB::varTable->getVarName(varID3) == "slalom");

    // Nested If vars
    ID condVarId3 = PKB::varTable->getVarID("a");
    REQUIRE(condVarId3 == 5);   // Gap in varID because of line 2 AssignStmt
    REQUIRE(PKB::varTable->getVarName(condVarId3) == "a");
    ID condVarId4 = PKB::varTable->getVarID("z");
    REQUIRE(condVarId4 == 6);
    REQUIRE(PKB::varTable->getVarName(condVarId4) == "z");

    ID varID4 = PKB::varTable->getVarID("quartz");
    REQUIRE(varID4 == 7);
    REQUIRE(PKB::varTable->getVarName(varID4) == "quartz");
    ID varID5 = PKB::varTable->getVarID("conspiracy");
    REQUIRE(varID5 == 8);
    REQUIRE(PKB::varTable->getVarName(varID5) == "conspiracy");
    ID varID6 = PKB::varTable->getVarID("sapphire");
    REQUIRE(varID6 == 9);
    REQUIRE(PKB::varTable->getVarName(varID6) == "sapphire");
    ID varID7 = PKB::varTable->getVarID("droning");
    REQUIRE(varID7 == 10);
    REQUIRE(PKB::varTable->getVarName(varID7) == "droning");
    ID varID8 = PKB::varTable->getVarID("IA");
    REQUIRE(varID8 == 11);
    REQUIRE(PKB::varTable->getVarName(varID8) == "IA");
    ID varID9 = PKB::varTable->getVarID("chalk");
    REQUIRE(varID9 == 12);
    REQUIRE(PKB::varTable->getVarName(varID9) == "chalk");

    // Check pattern storage
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getIfStmtsWithControlVar(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId3) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::stmtTable->getWhileStmtsWithControlVar(condVarId4) == unordered_set<StmtNum>{ 5 });

    // Check consts used in conditional
    REQUIRE(PKB::constTable->hasConst("5") == true);
    REQUIRE(PKB::constTable->getConstValue("5") == 5);
    REQUIRE(PKB::constTable->hasConst("10") == true);
    REQUIRE(PKB::constTable->getConstValue("10") == 10);
    REQUIRE(PKB::constTable->hasConst("-50") == false);
    REQUIRE(PKB::constTable->hasConst("33") == true);
    REQUIRE(PKB::constTable->getConstValue("33") == 33);
    REQUIRE(PKB::constTable->hasConst("1") == true);
    REQUIRE(PKB::constTable->getConstValue("1") == 1);
    REQUIRE(PKB::constTable->hasConst("100") == true);
    REQUIRE(PKB::constTable->getConstValue("100") == 100);

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) != 3); // 3 is nested in 2
    REQUIRE(PKB::follows->getFollower(2) != 4); // 4 nested in 2
    REQUIRE(PKB::follows->getFollower(3) != 4); // 4 in else
    REQUIRE(PKB::follows->getFollower(4) != 5); // 5 in another procedure
    REQUIRE(PKB::follows->getFollower(5) != 6); // 6 is nested in 5
    REQUIRE(PKB::follows->getFollower(5) != 7); // 7 is nested in 5
    REQUIRE(PKB::follows->getFollower(5) != 8); // 8 is nested in 5
    REQUIRE(PKB::follows->getFollower(5) != 9); // 9 is nested in 5
    REQUIRE(PKB::follows->getFollower(6) == 7); // nested in while loop
    REQUIRE(PKB::follows->getFollower(7) == 8); // nested in while loop
    REQUIRE(PKB::follows->getFollower(7) != 7); // stmt cannot follow itself
    REQUIRE(PKB::follows->getFollower(8) == 9); // nested in while loop
    REQUIRE(PKB::follows->getFollower(9) != 10); // 10 in another procedure
    REQUIRE(PKB::follows->getFollower(10) == 11);
    // Check Follows*
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<ID> { 2 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(5) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(6) == unordered_set<ID> { 7, 8, 9 });
    REQUIRE(PKB::follows->getFollowerStar(7) == unordered_set<ID> { 8, 9 });
    REQUIRE(PKB::follows->getFollowerStar(8) == unordered_set<ID> { 9 });
    REQUIRE(PKB::follows->getFollowerStar(9) == unordered_set<ID> { });
    REQUIRE(PKB::follows->getFollowerStar(10) == unordered_set<ID> { 11 });
    REQUIRE(PKB::follows->getFollowerStar(11) == unordered_set<ID> { });

    // Check Parent
    REQUIRE(PKB::parent->getChildren(2) == unordered_set<StmtNum>{ 3, 4 }); // Only testing for Parent, not Parent*
    REQUIRE(PKB::parent->getChildren(5) == unordered_set<StmtNum>{ 6, 7, 8, 9 });
    REQUIRE(PKB::parent->getParent(1) == -1);
    REQUIRE(PKB::parent->getParent(2) == -1);
    REQUIRE(PKB::parent->getParent(3) == 2);
    REQUIRE(PKB::parent->getParent(4) == 2);
    REQUIRE(PKB::parent->getParent(5) == -1);
    REQUIRE(PKB::parent->getParent(6) == 5);
    REQUIRE(PKB::parent->getParent(7) == 5);
    REQUIRE(PKB::parent->getParent(8) == 5);
    REQUIRE(PKB::parent->getParent(9) == 5);
    REQUIRE(PKB::parent->getParent(10) == -1);
    REQUIRE(PKB::parent->getParent(11) == -1);
    // Check Parent*
    REQUIRE(PKB::parent->getChildrenStar(1) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(2) == unordered_set<StmtNum>{ 3, 4 });
    REQUIRE(PKB::parent->getChildrenStar(3) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(4) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(5) == unordered_set<StmtNum>{ 6, 7, 8, 9 });
    REQUIRE(PKB::parent->getChildrenStar(6) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(7) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(8) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(9) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(10) == unordered_set<StmtNum>{ });
    REQUIRE(PKB::parent->getChildrenStar(11) == unordered_set<StmtNum>{ });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(condVarId) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId2) == unordered_set<StmtNum>{ 2 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId3) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::uses->getStmtsUses(condVarId4) == unordered_set<StmtNum>{ 5 });
    REQUIRE(PKB::uses->getStmtsUses(varID2) == unordered_set<StmtNum>{ 1, 2, 3, 4 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getStmtsUses(varID5) == unordered_set<StmtNum>{ 5, 6 }); // as 2 is a container stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(0) == unordered_set<StmtNum>{ condVarId, condVarId2, varID2 });
    // Uses includes uses relationships in call stmt
    REQUIRE(PKB::uses->getVarsUsedByProc(1) == unordered_set<StmtNum>{ condVarId3, condVarId4, varID5, varID9 });
    REQUIRE(PKB::uses->getVarsUsedByProc(2) == unordered_set<StmtNum>{ varID9 });

    // Check Modifies
    REQUIRE(PKB::modifies->getStmtsModifies(varID) == unordered_set<ID>{ 1 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID3) == unordered_set<ID>{ 2, 3, 4 }); // as 2 is a container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID4) == unordered_set<ID>{ 5, 6 }); // as 4 is container stmt
    REQUIRE(PKB::modifies->getStmtsModifies(varID6) == unordered_set<ID>{ 5, 7 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID7) == unordered_set<ID>{ 5, 8 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID8) == unordered_set<ID>{ 5, 9, 10 });

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });   // Next only holds for stmts in the same procedure
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ });   // Next only holds for stmts in the same procedure
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 6 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(9) == unordered_set<ProgLine>{ 5 });
    REQUIRE(PKB::next->getNext(10) == unordered_set<ProgLine>{ 11 });
    REQUIRE(PKB::next->getNext(11) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(9) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(10) == unordered_set<ProgLine>{ 11 });
    REQUIRE(PKB::next->getNextStar(11) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));
}

TEST_CASE("storeNewCall - nonexistent procedure") {
    // Assumption: nonexistent procedures being called will raise no error and no additional relationships are added
    DesignExtractor::signalReset();

    std::vector<sp::Token*> stubAssignTokens{
            // Assignment Stmt
            new sp::Token(sp::Token::TokenType::NAME, "scaramouche"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "tartaglia"),
            new sp::Token(sp::Token::TokenType::PLUS, "+"),
            new sp::Token(sp::Token::TokenType::NAME, "pustota"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "2"),
            new sp::Token(sp::Token::TokenType::TIMES, "*"),
            new sp::Token(sp::Token::TokenType::CONST, "0"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
            new sp::Token(sp::Token::TokenType::EOFF, "EOF")
    };

    auto lass = new LexerStub(stubAssignTokens);
    Parser pass = Parser(lass);
    ast::AssignStmt* assStmt = pass.parseAssignStmt();

    // Create call AST
    STRING input3 = "call abyssal;";
    std::vector<sp::Token> actual_tok3;
    std::vector<sp::Token*> tok_ptrs3;
    ParserUtils::StringToTokenPtrs(input3, actual_tok3, tok_ptrs3);
    auto lCall = new LexerStub(tok_ptrs3);
    auto pCall = Parser(lCall);
    ast::CallStmt* callStmt = pCall.parseCallStmt();

    DesignExtractor::storeNewProcedure("strobelight");
    DesignExtractor::storeNewPrint(1, "reason", createPrint("reason"));
    DesignExtractor::storeNewRead(2, "dyed", createRead("dyed"));
    DesignExtractor::storeNewAssignment(3, "scaramouche", assStmt);
    REQUIRE(DesignExtractor::storeNewCall(4, "strobelight", "abyssal", callStmt) == true);
    DesignExtractor::exitProcedure();
    DesignExtractor::storeNewProcedure("abyssal"); // dummy
    DesignExtractor::storeNewPrint(5, "x", createPrint("x"));
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check procTable
    ID procID = PKB::procTable->getProcID("strobelight");
    REQUIRE(PKB::procTable->hasProc("strobelight") == true);
    REQUIRE(procID == 0);
    // Check StmtLst
    REQUIRE(PKB::stmtLstTable->hasStmtLst(1) == true);

    // Check varNames
    ID varID = PKB::varTable->getVarID("reason");
    REQUIRE(varID == 0);    // varTable ID starts indexing at 0
    REQUIRE(PKB::varTable->getVarName(varID) == "reason");
    ID varID1 = PKB::varTable->getVarID("dyed");
    REQUIRE(varID1 == 1);
    REQUIRE(PKB::varTable->getVarName(varID1) == "dyed");
    ID varID2 = PKB::varTable->getVarID("scaramouche");
    REQUIRE(varID2 == 2);
    REQUIRE(PKB::varTable->getVarName(varID2) == "scaramouche");
    ID varID3 = PKB::varTable->getVarID("tartaglia");
    REQUIRE(varID3 == 3);
    REQUIRE(PKB::varTable->getVarName(varID3) == "tartaglia");
    ID varID4 = PKB::varTable->getVarID("pustota");
    REQUIRE(varID4 == 4);
    REQUIRE(PKB::varTable->getVarName(varID4) == "pustota");
    ID varID5 = PKB::varTable->getVarID("x");
    REQUIRE(varID5 == 5);
    REQUIRE(PKB::varTable->getVarName(varID5) == "x");

    // Check Follows
    REQUIRE(PKB::follows->getFollower(1) == 2);
    REQUIRE(PKB::follows->getFollower(2) == 3);
    REQUIRE(PKB::follows->getFollower(2) != 4);
    REQUIRE(PKB::follows->getFollower(0) == -1);
    REQUIRE(PKB::follows->getFollower(10) == -1);
    // Check Follows*
    REQUIRE(PKB::follows->getFollowerStar(1) == unordered_set<ID> { 2, 3, 4 });
    REQUIRE(PKB::follows->getFollowerStar(2) == unordered_set<ID> { 3, 4 });
    REQUIRE(PKB::follows->getFollowerStar(3) == unordered_set<ID> { 4 });
    REQUIRE(PKB::follows->getFollowerStar(4) == unordered_set<ID> { });

    // Check Modifies
    // Assign Stmt
    REQUIRE(PKB::modifies->getProcsModifies(varID2) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID2) == unordered_set<StmtNum>{ 3 });
    // Read Stmt
    REQUIRE(PKB::modifies->getProcsModifies(varID1) == unordered_set<ID>{ 0 });
    REQUIRE(PKB::modifies->getStmtsModifies(varID1) == unordered_set<StmtNum>{ 2 });

    // Check Uses
    REQUIRE(PKB::uses->getStmtsUses(varID) == unordered_set<StmtNum>{ 1 });
    REQUIRE(PKB::uses->getStmtsUses(varID3) == unordered_set<StmtNum>{ 3 });
    REQUIRE(PKB::uses->getStmtsUses(varID4) == unordered_set<StmtNum>{ 3 });
    REQUIRE(PKB::uses->getVarsUsedByProc(procID) == unordered_set<StmtNum>{ varID, varID3, varID4, varID5 });
}

TEST_CASE("Next/* Variants for If-Else and While Statements") {
    // AST value does not matter at all, as we are only testing for Next correctness
    // Set up Assignment AST
    std::vector<sp::Token*> stubTokens{
            new sp::Token(sp::Token::TokenType::NAME, "axel2"),
            new sp::Token(sp::Token::TokenType::ASSIGN, "="),
            new sp::Token(sp::Token::TokenType::NAME, "semelparity"),
            new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
    };
    auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    Parser p = Parser(l);
    ast::AssignStmt* assignment = p.parseAssignStmt();

    // StmtLst inputs are just dummies, this is not the exact statement being tested
    STRING input = "if ((x == 5) || (y > 10)) then { dummy =  1; } else { dummy =  1; }";
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lif = new LexerStub(tok_ptrs);
    auto pif = Parser(lif);
    ast::IfStmt* ifStmt = pif.parseIfStmt();

    STRING input2 = "while ((a != 33) || (z > 1)) { dummy =  1; }";
    std::vector<sp::Token> actual_tok2;
    std::vector<sp::Token*> tok_ptrs2;
    ParserUtils::StringToTokenPtrs(input2, actual_tok2, tok_ptrs2);
    auto lNestedWhile = new LexerStub(tok_ptrs2);
    auto pNestedWhile = Parser(lNestedWhile);
    ast::WhileStmt* whileStmt = pNestedWhile.parseWhileStmt();
    ast::WhileStmt* nestedWhileStmt = whileStmt;

    // Create call AST
    STRING input3 = "call abyssal;";
    std::vector<sp::Token> actual_tok3;
    std::vector<sp::Token*> tok_ptrs3;
    ParserUtils::StringToTokenPtrs(input3, actual_tok3, tok_ptrs3);
    auto lCall = new LexerStub(tok_ptrs3);
    auto pCall = Parser(lCall);
    ast::CallStmt* callStmt = pCall.parseCallStmt();

    ReadStmt *readStmt = createRead("droning");

    vector<STRING> condVarNames{ "x", "y" };    // Used in conditional expression of if stmt
    vector<STRING> condConsts{ "5", "10" };
    vector<STRING> nestedCondVarNames{ "a", "z" };    // Used in conditional expression of nested if stmt
    vector<STRING> nestedCondConsts{ "33", "1" };

    DesignExtractor::signalReset();
    // Tests DE's ability to extract Next for a nested if statement with explicit endpoints in the CFG
    DesignExtractor::storeNewProcedure("mitosis");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewIf(4,nestedCondVarNames, nestedCondConsts, ifStmt);    // Nested If-then
    DesignExtractor::storeNewAssignment(5, "quartz", assignment);
    DesignExtractor::storeNewElse();                                                    // Nested Else
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment);
    DesignExtractor::storeNewRead(7, "droning", readStmt);
    DesignExtractor::endIfElse();
    DesignExtractor::storeNewRead(8, "droning", readStmt);
    DesignExtractor::endIfElse();
    DesignExtractor::storeNewRead(9, "droning", readStmt);
    DesignExtractor::storeNewCall(10, "deux", "abyssal", callStmt);
    DesignExtractor::exitProcedure();
    DesignExtractor::storeNewProcedure("abyssal"); // dummy
    DesignExtractor::storeNewRead(11, "x", readStmt);
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(9) == unordered_set<ProgLine>{ 10 });
    REQUIRE(PKB::next->getNext(10) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7, 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 8, 9, 10 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 9, 10 });
    REQUIRE(PKB::next->getNextStar(9) == unordered_set<ProgLine>{ 10 });
    REQUIRE(PKB::next->getNextStar(10) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{8}));
    REQUIRE(PKB::stmtTable->getIfStmtRange(4) == make_pair(5, set<int>{5}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(4) == make_pair(6, set<int>{7}));

    DesignExtractor::signalReset();
    // Tests DE's ability to extract Next for a nested if statement with no explicit endpoint at all
    DesignExtractor::storeNewProcedure("mitosis");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewIf(4,nestedCondVarNames, nestedCondConsts, ifStmt);    // Nested If-then
    DesignExtractor::storeNewAssignment(5, "quartz", assignment);
    DesignExtractor::storeNewElse();                                                    // Nested Else
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment);
    DesignExtractor::storeNewRead(7, "droning", readStmt);
    DesignExtractor::endIfElse();
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{5, 7}));
    REQUIRE(PKB::stmtTable->getIfStmtRange(4) == make_pair(5, set<int>{5}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(4) == make_pair(6, set<int>{7}));

    DesignExtractor::signalReset();
    // Tests DE's ability to extract Next for nested if statements with no explicit 'end' point in the CFG for outer if
    DesignExtractor::storeNewProcedure("mitosis");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewIf(4,nestedCondVarNames, nestedCondConsts, ifStmt);    // Nested If-then
    DesignExtractor::storeNewAssignment(5, "quartz", assignment);
    DesignExtractor::storeNewElse();                                                    // Nested Else
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment);
    DesignExtractor::storeNewRead(7, "droning", readStmt);
    DesignExtractor::endIfElse();
    DesignExtractor::endIfElse();
    DesignExtractor::storeNewRead(8, "droning", readStmt);
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5, 6 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 8 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7, 8 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7, 8 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 8 });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{5, 7}));
    REQUIRE(PKB::stmtTable->getIfStmtRange(4) == make_pair(5, set<int>{5}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(4) == make_pair(6, set<int>{7}));

    DesignExtractor::signalReset();
    // Two if statements, not nested
    DesignExtractor::storeNewProcedure("mitosis");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewRead(4, "droning", readStmt);
    DesignExtractor::endIfElse();

    DesignExtractor::storeNewIf(5,nestedCondVarNames, nestedCondConsts, ifStmt);    //  If-then
    DesignExtractor::storeNewAssignment(6, "quartz", assignment);
    DesignExtractor::storeNewCall(7, "deux", "abyssal", callStmt);
    DesignExtractor::storeNewElse();                                                    //  Else
    DesignExtractor::storeNewAssignment(8, "sapphire", assignment);
    DesignExtractor::storeNewRead(9, "droning", readStmt);
    DesignExtractor::endIfElse();

    DesignExtractor::exitProcedure();

    DesignExtractor::storeNewProcedure("abyssal"); // dummy
    DesignExtractor::storeNewRead(10, "x", readStmt);
    DesignExtractor::exitProcedure();

    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 5 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 6, 8 }); // 2nd procedure
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNext(9) == unordered_set<ProgLine>{ });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 5, 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 6, 7, 8, 9 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(8) == unordered_set<ProgLine>{ 9 });
    REQUIRE(PKB::next->getNextStar(9) == unordered_set<ProgLine>{ });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));
    REQUIRE(PKB::stmtTable->getIfStmtRange(5) == make_pair(6, set<int>{7}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(5) == make_pair(8, set<int>{9}));

    DesignExtractor::signalReset();
    // Nested while-if
    DesignExtractor::storeNewProcedure("arabesque");
    DesignExtractor::storeNewAssignment(1, "axel2", assignment);
    DesignExtractor::storeNewIf(2,condVarNames, condConsts, ifStmt);    // If-then
    DesignExtractor::storeNewAssignment(3, "slalom", assignment);
    DesignExtractor::storeNewElse();                                                    // Else
    DesignExtractor::storeNewWhile(4,nestedCondVarNames, nestedCondConsts, nestedWhileStmt);    // Nested While
    DesignExtractor::storeNewAssignment(5, "quartz", assignment);
    DesignExtractor::storeNewAssignment(6, "sapphire", assignment);
    DesignExtractor::storeNewRead(7, "droning", readStmt);
    DesignExtractor::exitWhile();
    DesignExtractor::endIfElse();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3, 4 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 5 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 6 });
    REQUIRE(PKB::next->getNext(6) == unordered_set<ProgLine>{ 7 });
    REQUIRE(PKB::next->getNext(7) == unordered_set<ProgLine>{ 4 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 2, 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 3, 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(5) == unordered_set<ProgLine>{ 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(6) == unordered_set<ProgLine>{ 4, 5, 6, 7 });
    REQUIRE(PKB::next->getNextStar(7) == unordered_set<ProgLine>{ 4, 5, 6, 7 });

    // Check if/else storage
    REQUIRE(PKB::stmtTable->getIfStmtRange(2) == make_pair(3, set<int>{3}));
    REQUIRE(PKB::stmtTable->getElseStmtRange(2) == make_pair(4, set<int>{4}));

    DesignExtractor::signalReset();
    // Simple While
    DesignExtractor::storeNewProcedure("hana");
    DesignExtractor::storeNewWhile(1,condVarNames, condConsts, whileStmt);
    DesignExtractor::storeNewAssignment(2, "axel2", assignment);
    DesignExtractor::storeNewRead(3, "shine", readStmt);
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 1 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 1, 2, 3 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 1, 2, 3 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 1, 2, 3 });

    DesignExtractor::signalReset();
    // 1 nested While with no explicit endpoint
    DesignExtractor::storeNewProcedure("hana");
    DesignExtractor::storeNewWhile(1,condVarNames, condConsts, whileStmt);
    DesignExtractor::storeNewAssignment(2, "axel2", assignment);
    DesignExtractor::storeNewWhile(3,nestedCondVarNames, nestedCondConsts, nestedWhileStmt);
    DesignExtractor::storeNewRead(4, "shine", readStmt);
    DesignExtractor::exitWhile();
    DesignExtractor::exitWhile();
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 1, 4 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 3 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 1, 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 1, 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 1, 2, 3, 4 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 1, 2, 3, 4 });

    DesignExtractor::signalReset();
    // 1 nested While
    DesignExtractor::storeNewProcedure("hana");
    DesignExtractor::storeNewWhile(1,condVarNames, condConsts, whileStmt);
    DesignExtractor::storeNewAssignment(2, "axel2", assignment);
    DesignExtractor::storeNewWhile(3,nestedCondVarNames, nestedCondConsts, nestedWhileStmt);
    DesignExtractor::storeNewRead(4, "shine", readStmt);
    DesignExtractor::exitWhile();
    DesignExtractor::storeNewRead(5, "shine2", readStmt);
    DesignExtractor::exitWhile();
    DesignExtractor::storeNewRead(6, "shine3", readStmt);
    DesignExtractor::exitProcedure();
    REQUIRE(DesignExtractor::signalEnd() == true);

    // Check Next
    REQUIRE(PKB::next->getNext(1) == unordered_set<ProgLine>{ 2, 6 });
    REQUIRE(PKB::next->getNext(2) == unordered_set<ProgLine>{ 3 });
    REQUIRE(PKB::next->getNext(3) == unordered_set<ProgLine>{ 4, 5 });
    REQUIRE(PKB::next->getNext(4) == unordered_set<ProgLine>{ 3 });
    REQUIRE(PKB::next->getNext(5) == unordered_set<ProgLine>{ 1 });
    // Check Next*
    REQUIRE(PKB::next->getNextStar(1) == unordered_set<ProgLine>{ 1, 2, 3, 4, 5, 6 });
    REQUIRE(PKB::next->getNextStar(2) == unordered_set<ProgLine>{ 1, 2, 3, 4, 5, 6 });
    REQUIRE(PKB::next->getNextStar(3) == unordered_set<ProgLine>{ 1, 2, 3, 4, 5, 6 });
    REQUIRE(PKB::next->getNextStar(4) == unordered_set<ProgLine>{ 1, 2, 3, 4, 5, 6 });
}
