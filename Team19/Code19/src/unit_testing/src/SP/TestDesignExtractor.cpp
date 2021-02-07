#include "SP/DesignExtractor.h"
#include "PKB/PKB.h"
#include "AST/Index.h"
#include "SP/Parser.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("storeNewProcedure Test") {

}

TEST_CASE("storeNewAssignment Const RHS Test") {
    PKB::resetPKB();
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
    PKB::resetPKB();
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

TEST_CASE("storeNewAssignment VarNames and Consts Test") {

}


// Test cases for stacks and related methods
TEST_CASE("Generic Stack Operations Test") {
    vector<vector<int>> stmtLstsStack;
    vector<set<ID>> usesStack;
    vector<set<ID>> modifiesStack;
    vector<int> parentStack;

    // Test stmtLstsStack
    vector<ID> numbers{ 0, 1, 3, 400, 70000};
    // -3 is an invalid stmtNum, but DesignExtractor assumes all inputs from Parser are valid.
    vector<ID> numbers2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(stmtLstsStack.size() == 0);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    REQUIRE(stmtLstsStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == numbers);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == vector<ID>{});
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers);
    DesignExtractor::DEStack<vector<int>>::stackPush(stmtLstsStack, numbers2);
    REQUIRE(stmtLstsStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<vector<ID>>::stackPop(stmtLstsStack) == numbers2);

    // Test usesStack
    set<ID> usesIDs{ 0, 1, 3, 400, 70000};
    set<ID> usesIDs2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(usesStack.size() == 0);
    DesignExtractor::DEStack<set<ID>>::stackPush(usesStack, usesIDs);
    REQUIRE(usesStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == usesIDs);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == set<ID>{});
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(usesStack, usesIDs2);
    REQUIRE(usesStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(usesStack) == usesIDs2);

    // Test modifiesStack
    set<ID> modifiesIDs{ 0, 1, 3, 400, 70000};
    set<ID> modifiesIDs2{ INT_MAX, 0, 1, -3, 400, 809};
    REQUIRE(modifiesStack.size() == 0);
    DesignExtractor::DEStack<set<ID>>::stackPush(modifiesStack, usesIDs);
    REQUIRE(modifiesStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == usesIDs);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == set<ID>{});
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs);
    DesignExtractor::DEStack<set<int>>::stackPush(modifiesStack, usesIDs2);
    REQUIRE(modifiesStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<set<ID>>::stackPop(modifiesStack) == usesIDs2);

    // Test parentStack
    REQUIRE(parentStack.size() == 0);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 3);
    REQUIRE(parentStack.size() == 1);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 3);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 1);  // empty parent stack returns 1
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 5);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 356);
    DesignExtractor::DEStack<ID>::stackPush(parentStack, 909090);
    REQUIRE(parentStack.size() == 3);
    REQUIRE(DesignExtractor::DEStack<ID>::stackPop(parentStack) == 909090);
};

