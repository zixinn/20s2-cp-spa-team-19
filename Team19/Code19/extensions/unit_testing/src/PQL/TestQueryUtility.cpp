#include "PQL/QueryUtility.h"
#include "catch.hpp"

#include <set>

using namespace std;

TEST_CASE("checkNameWithQuotes") {
    // valid name
    REQUIRE(checkNameWithQuotes("\"name\""));
    REQUIRE(checkNameWithQuotes("\"NaMe\""));
    REQUIRE(checkNameWithQuotes("\"N4m3\""));
    REQUIRE(checkNameWithQuotes("\"a\""));

    // invalid name
    REQUIRE_FALSE(checkNameWithQuotes("\"1name\""));
    REQUIRE_FALSE(checkNameWithQuotes("\"my_name\""));
    REQUIRE_FALSE(checkNameWithQuotes("\"my name\""));
    REQUIRE_FALSE(checkNameWithQuotes("\"name?\""));
    REQUIRE_FALSE(checkNameWithQuotes("\"n@me\""));

    REQUIRE_FALSE(checkNameWithQuotes("name"));
    REQUIRE_FALSE(checkNameWithQuotes("\"NaMe"));
    REQUIRE_FALSE(checkNameWithQuotes("N4m3\""));
    REQUIRE_FALSE(checkNameWithQuotes("\"a\"\""));
}

TEST_CASE("isOperator") {
    // valid operator
    REQUIRE(isOperator(sp::Token::TokenType::PLUS));
    REQUIRE(isOperator(sp::Token::TokenType::MINUS));
    REQUIRE(isOperator(sp::Token::TokenType::TIMES));
    REQUIRE(isOperator(sp::Token::TokenType::DIV));
    REQUIRE(isOperator(sp::Token::TokenType::MOD));

    // invalid operator
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::EOFF));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::ASSIGN));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::CONST));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::OR));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::EQ));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::NOT));
    REQUIRE_FALSE(isOperator(sp::Token::TokenType::RPAREN));
}

TEST_CASE("checkExpression") {
    // valid expression
    REQUIRE(checkExpression("\"1 \""));
    REQUIRE(checkExpression("\"2 + 3\""));
    REQUIRE(checkExpression("\"  xyz-abc\""));
    REQUIRE(checkExpression("\"w   * 4\""));
    REQUIRE(checkExpression("\"(def) % (if + 5)\""));

    // invalid expression
    REQUIRE_FALSE(checkExpression("\")\""));
    REQUIRE_FALSE(checkExpression("\"()\""));
    REQUIRE_FALSE(checkExpression("\"+9\""));
    REQUIRE_FALSE(checkExpression("\"8+-7\""));
    REQUIRE_FALSE(checkExpression("\"[6 / 5]\""));
    REQUIRE_FALSE(checkExpression("\"(4 * (x % 3)\""));
    REQUIRE_FALSE(checkExpression("\"2 - ?\""));
    REQUIRE_FALSE(checkExpression("\"+1-0\""));
    REQUIRE_FALSE(checkExpression("\"__\""));
    REQUIRE_FALSE(checkExpression("\"2+\""));
    REQUIRE_FALSE(checkExpression("\"red1 < red2\""));
    REQUIRE_FALSE(checkExpression("\"red1 red2\""));
}

TEST_CASE("checkExpressionWithUnderscores") {
    // valid expression with underscores
    REQUIRE(checkExpressionWithUnderscores("_\"1 \"_"));
    REQUIRE(checkExpressionWithUnderscores("_  \"2 + 3\"_"));
    REQUIRE(checkExpressionWithUnderscores("_\"  xyz-abc\"  _"));
    REQUIRE(checkExpressionWithUnderscores("_ \"w   * 4\" _"));
    REQUIRE(checkExpressionWithUnderscores("_\"(def) % (if + 5)\"_"));

    // invalid expression with underscores
    REQUIRE_FALSE(checkExpressionWithUnderscores("_\"()\"_"));
    REQUIRE_FALSE(checkExpressionWithUnderscores("_ \"+9\"_"));
    REQUIRE_FALSE(checkExpressionWithUnderscores("_\"8+-7\" _"));
    REQUIRE_FALSE(checkExpressionWithUnderscores("\"6 + 5\""));
    REQUIRE_FALSE(checkExpressionWithUnderscores("_\"abc * def\""));
    REQUIRE_FALSE(checkExpressionWithUnderscores("__\"xyz / 4\""));
    REQUIRE_FALSE(checkExpressionWithUnderscores("\"3\"_"));
}

TEST_CASE("checkSynonymDeclared") {
    unordered_map<STRING, STRING> declarations = {{"a", "assign"}, {"v", "variable"}};
    checkSynonymDeclared("a", declarations);
    REQUIRE(checkSynonymDeclared("a", declarations));
    REQUIRE_FALSE(checkSynonymDeclared("w", declarations));
}

TEST_CASE("getArgType") {
    unordered_map<STRING, STRING> declarations = {{"a", "assign"}, {"v", "variable"}};
    REQUIRE(getArgType("v", declarations) == VARIABLE_);
    REQUIRE(getArgType("123", declarations) == INTEGER_);
    REQUIRE(getArgType("_", declarations) == UNDERSCORE_);
    REQUIRE(getArgType("\"x\"", declarations) == NAME_);
    REQUIRE(getArgType("\"x + y * z\"", declarations) == EXPRESSION_);
    REQUIRE(getArgType("_\"x + y * z\"_", declarations) == EXPRESSIONWITHUNDERSCORE_);
    REQUIRE(getArgType("s", declarations) == "");
    REQUIRE(getArgType("\"_\"", declarations) == "");
}

class StmtNodeStub : public ast::Stmt {
public:
    StmtNodeStub(StmtNum index): ast::Stmt(new sp::Token(), index){};
};

void setupPKB() {
    PKB::constTable = new ConstTable();
    PKB::constTable->storeConst("2");
    PKB::constTable->storeConst("8");

    PKB::varTable = new VarTable();
    PKB::varTable->storeVarName("x");
    PKB::varTable->storeVarName("y");
    PKB::varTable->storeVarName("z");

    PKB::procTable = new ProcTable();
    PKB::procTable->storeProcName("main");
    PKB::procTable->storeProcName("compute");

    PKB::stmtTable = new StmtTable();
    ast::Stmt* stmtNodeStub1 = new StmtNodeStub(1);
    ast::Stmt* stmtNodeStub2 = new StmtNodeStub(2);
    ast::Stmt* stmtNodeStub3 = new StmtNodeStub(3);
    ast::Stmt* stmtNodeStub4 = new StmtNodeStub(4);
    ast::Stmt* stmtNodeStub5 = new StmtNodeStub(5);
    ast::Stmt* stmtNodeStub6 = new StmtNodeStub(6);
    ast::Stmt* stmtNodeStub7 = new StmtNodeStub(7);
    ast::Stmt* stmtNodeStub8 = new StmtNodeStub(8);
    PKB::stmtTable->storeStmt(1, stmtNodeStub1, ASSIGN_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub2, READ_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub3, PRINT_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub4, CALL_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub5, WHILE_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub6, IF_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub7, ASSIGN_);
    PKB::stmtTable->storeStmt(8, stmtNodeStub8, READ_);
}

TEST_CASE("selectAll") {
    setupPKB();
    vector<int> v1 = selectAll(PROCEDURE_);
    unordered_set<int> s11(v1.begin(), v1.end());
    unordered_set<int> s12 = { 0, 1 };
    REQUIRE( s11 == s12);
    vector<int> v2 = selectAll(STMT_);
    unordered_set<int> s21(v2.begin(), v2.end());
    unordered_set<int> s22 = { 1, 2, 3, 4, 5, 6, 7, 8 };
    REQUIRE( s21 == s22 );
    vector<int> v3 = selectAll(READ_);
    unordered_set<int> s31(v3.begin(), v3.end());
    unordered_set<int> s32 = { 2, 8 };
    REQUIRE( s31 == s32 );
    REQUIRE(selectAll(PRINT_) == vector<int>{3});
    vector<int> v4 = selectAll(ASSIGN_);
    unordered_set<int> s41(v4.begin(), v4.end());
    unordered_set<int> s42 = { 1, 7 };
    REQUIRE( s41 == s42 );
    REQUIRE(selectAll(CALL_) == vector<int>{4});
    REQUIRE(selectAll(WHILE_) == vector<int>{5});
    REQUIRE(selectAll(IF_) == vector<int>{6});
    vector<int> v5 = selectAll(VARIABLE_);
    unordered_set<int> s51(v5.begin(), v5.end());
    unordered_set<int> s52 = { 0, 1, 2 };
    REQUIRE( s51 == s52 );
    vector<int> v6 = selectAll(CONSTANT_);
    unordered_set<int> s61(v6.begin(), v6.end());
    unordered_set<int> s62 = { 2, 8 };
    REQUIRE( s61 == s62 );
    REQUIRE(selectAll(UNDERSCORE_).empty());
}

TEST_CASE("intersectSingleSynonym") {
    // no duplicates
    vector<int> allResults1{1, 2, 3, 4, 5, 6};
    vector<int> allCorrectType1{0, 2, 4, 6, 8};
    vector<int> results1;
    bool nonEmpty1 = intersectSingleSynonym(allResults1, allCorrectType1, results1);
    REQUIRE(nonEmpty1);
    vector<int> expected1{2, 4, 6};
    REQUIRE(unordered_set<int>(results1.begin(), results1.end()) == unordered_set<int>(expected1.begin(), expected1.end()));

    // with duplicates
    vector<int> allResults2{1, 2, 3, 3, 4, 5, 6, 4};
    vector<int> allCorrectType2{0, 2, 4, 6, 8};
    vector<int> results2;
    bool nonEmpty2 = intersectSingleSynonym(allResults2, allCorrectType2, results2);
    REQUIRE(nonEmpty2);
    vector<int> expected2{2, 4, 6};
    REQUIRE(unordered_set<int>(results2.begin(), results2.end()) == unordered_set<int>(expected2.begin(), expected2.end()));

    // empty intersection
    vector<int> allResults3{1, 3, 3, 5};
    vector<int> allCorrectType3{0, 2, 4, 6, 8};
    vector<int> results3;
    bool nonEmpty3 = intersectSingleSynonym(allResults3, allCorrectType3, results3);
    REQUIRE_FALSE(nonEmpty3);
    REQUIRE(results3.empty());
}

TEST_CASE("intersectDoubleSynonym") {
    // non-empty intersection
    pair<vector<int>, vector<int>> allResults1 = make_pair(vector<int>{1, 3, 5, 7}, vector<int>{2, 4, 6, 8});
    pair<vector<int>, vector<int>> allCorrectType1 = make_pair(vector<int>{5, 1, 11}, vector<int>{8, 2, 6, 22, 4});
    pair<vector<int>, vector<int>> results1;
    bool nonEmpty1 = intersectDoubleSynonym(allResults1, allCorrectType1, results1);
    REQUIRE(nonEmpty1);
    pair<vector<int>, vector<int>> expected1 = make_pair(vector<int>{1, 5}, vector<int>{2, 6});
    REQUIRE(results1 == expected1);

    // empty intersection
    pair<vector<int>, vector<int>> allResults2 = make_pair(vector<int>{1, 3, 5, 7}, vector<int>{2, 4, 6, 8});
    pair<vector<int>, vector<int>> allCorrectType2 = make_pair(vector<int>{1, 5, 9}, vector<int>{4, 0});
    pair<vector<int>, vector<int>> results2;
    bool nonEmpty2 = intersectDoubleSynonym(allResults2, allCorrectType2, results2);
    REQUIRE_FALSE(nonEmpty2);
    REQUIRE(results2 == make_pair(vector<int>{}, vector<int>{}));
}

TEST_CASE("project") {
    // empty result (all columns removed)
    unordered_set<STRING> toProject1 {"a", "v"};
    unordered_map<STRING, vector<int>> results1;
    results1["c"] = {9, 8, 7, 6, 5, 4, 3};
    results1["n"] = {1, 2, 3, 4, 5, 6, 7};
    project(toProject1, results1);
    REQUIRE(results1.empty());

    // non-empty result (some columns removed), duplicates
    unordered_set<STRING> toProject2 {"a", "v"};
    unordered_map<STRING, vector<int>> results2;
    results2["a"] = {1, 3, 5, 3, 7, 7, 5};
    results2["c"] = {9, 8, 7, 6, 5, 4, 3};
    results2["v"] = {2, 4, 6, 4, 8, 9, 6};
    results2["n"] = {1, 2, 3, 4, 5, 6, 7};
    project(toProject2, results2);
    REQUIRE(results2.size() == 2);
    REQUIRE(results2["a"] == vector<int>{1, 3, 5, 7, 7});
    REQUIRE(results2["v"] == vector<int>{2, 4, 6, 8, 9});

    // no columns removed, no duplicates
    unordered_set<STRING> toProject3 {"c", "n"};
    unordered_map<STRING, vector<int>> results3;
    results3["c"] = {9, 8, 7, 6, 5, 4, 3};
    results3["n"] = {1, 2, 3, 4, 5, 6, 7};
    project(toProject3, results3);
    REQUIRE(results3.size() == 2);
    REQUIRE(results3["c"] == vector<int>{9, 8, 7, 6, 5, 4, 3});
    REQUIRE(results3["n"] == vector<int>{1, 2, 3, 4, 5, 6, 7});

    // no columns removed, duplicates
    unordered_set<STRING> toProject4 {"a", "v"};
    unordered_map<STRING, vector<int>> results4;
    results4["a"] = {1, 3, 5, 3, 7, 7, 5};
    results4["v"] = {2, 4, 6, 4, 8, 9, 6};
    project(toProject4, results4);
    REQUIRE(results4.size() == 2);
    REQUIRE(results4["a"] == vector<int>{1, 3, 5, 7, 7});
    REQUIRE(results4["v"] == vector<int>{2, 4, 6, 8, 9});
}

//    procedure sumDigits {
//    1.  read number;
//    2.  sum = 0;
//    3.  while (number > 0) {
//    4.      digit = number % 10;
//    5.      sum = sum + digit;
//    6.      number = number / 10;
//        }
//    7.  print sum;
//    }

void setupPKB2() {
    PKB::resetPKB();

    ast::Stmt* stmtNodeStub = new StmtNodeStub(0);

    PKB::procTable->storeProcName("sumDigits"); // 0
    PKB::procTable->storeProcStmt(0, 1, 7);

    PKB::varTable->storeVarName("number"); // 0
    PKB::varTable->storeVarName("sum"); // 1
    PKB::varTable->storeVarName("digit"); // 2

    PKB::constTable->storeConst("0");
    PKB::constTable->storeConst("10");

    PKB::stmtTable->storeStmt(1, stmtNodeStub, READ_);
    PKB::stmtTable->storeStmt(2, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(3, stmtNodeStub, WHILE_);
    PKB::stmtTable->storeStmt(4, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(5, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(6, stmtNodeStub, ASSIGN_);
    PKB::stmtTable->storeStmt(7, stmtNodeStub, PRINT_);

    PKB::follows->storeFollows(1, 2);
    PKB::follows->storeFollows(2, 3);
    PKB::follows->storeFollows(4, 5);
    PKB::follows->storeFollows(5, 6);
    PKB::follows->storeFollows(3, 7);

    PKB::parent->storeParent(3, 4);
    PKB::parent->storeParent(3, 5);
    PKB::parent->storeParent(3, 6);

    PKB::uses->storeStmtUses(3, 0);
    PKB::uses->storeStmtUses(3, 1);
    PKB::uses->storeStmtUses(3, 2);
    PKB::uses->storeStmtUses(4, 0);
    PKB::uses->storeStmtUses(5, 1);
    PKB::uses->storeStmtUses(5, 2);
    PKB::uses->storeStmtUses(6, 0);
    PKB::uses->storeStmtUses(7, 1);
    PKB::uses->storeProcUses(0, 0);
    PKB::uses->storeProcUses(0, 1);
    PKB::uses->storeProcUses(0, 2);

    PKB::modifies->storeStmtModifies(1, 0);
    PKB::modifies->storeStmtModifies(2, 1);
    PKB::modifies->storeStmtModifies(3, 0);
    PKB::modifies->storeStmtModifies(3, 1);
    PKB::modifies->storeStmtModifies(3, 2);
    PKB::modifies->storeStmtModifies(4, 2);
    PKB::modifies->storeStmtModifies(5, 1);
    PKB::modifies->storeStmtModifies(6, 0);
    PKB::modifies->storeProcModifies(0, 1);
    PKB::modifies->storeProcModifies(0, 2);
    PKB::modifies->storeProcModifies(0, 3);

    PKB::next->storeNext(1, 2);
    PKB::next->storeNext(2, 3);
    PKB::next->storeNext(3, 4);
    PKB::next->storeNext(3, 7);
    PKB::next->storeNext(4, 5);
    PKB::next->storeNext(5, 6);
    PKB::next->storeNext(6, 7);
    PKB::next->storeNext(6, 3);

    PKB::nextBip->setRunNextBip(true);
    PKB::affectsBip->setRunAffectsBip(true);
    PKB::populatePKB();
}

TEST_CASE("getSize") {
    setupPKB2();
    REQUIRE(PKB::follows->getFollowsSize() == 5);
    REQUIRE(PKB::follows->getFollowsStarSize() == 9);
    REQUIRE(PKB::parent->getParentSize() == 3);
    REQUIRE(PKB::parent->getParentStarSize() == 3);
    REQUIRE(PKB::uses->getProcSize() == 3);
    REQUIRE(PKB::uses->getStmtSize() == 8);
    REQUIRE(PKB::modifies->getProcSize() == 3);
    REQUIRE(PKB::modifies->getStmtSize() == 8);
    REQUIRE(PKB::calls->getCallsSize() == 0);
    REQUIRE(PKB::calls->getCallsStarSize() == 0);
    REQUIRE(PKB::next->getNextSize() == 8);
    REQUIRE(PKB::next->getNextStarSize() == 31);
    REQUIRE(PKB::affects->getAffectsSize() == 5);
    REQUIRE(PKB::affects->getAffectsStarSize() == 6);
    REQUIRE(PKB::nextBip->getNextBipSize() == 8);
    REQUIRE(PKB::nextBip->getNextBipStarSize() == 31);
    REQUIRE(PKB::affectsBip->getAffectsBipSize() == 5);
    REQUIRE(PKB::affectsBip->getAffectsBipStarSize() == 6);
    REQUIRE(PKB::procTable->getSize() == 1);
    REQUIRE(PKB::varTable->getSize() == 3);
    REQUIRE(PKB::constTable->getSize() == 2);
    REQUIRE(PKB::stmtTable->getSize() == 7);
    REQUIRE(PKB::stmtTable->getAllAssignStmtNums().size() == 4);
    REQUIRE(PKB::stmtTable->getAllWhileStmtNums().size() == 1);
    REQUIRE(PKB::stmtTable->getAllIfStmtNums().size() == 0);

    unordered_map<STRING, STRING> declarations = {{"s", STMT_}, {"r", READ_}, {"pn", PRINT_}, {"a", ASSIGN_},
                                                  {"c", CALL_}, {"w", WHILE_}, {"ifs", IF_}, {"v", VARIABLE_},
                                                  {"cc", CONSTANT_}, {"p", PROCEDURE_}, {"n", PROGLINE_}};

    Clause c1 = Clause("Follows", {"6", "s"}, {"s"}, 1);
    REQUIRE(getSize(c1, declarations) == 5);

    Clause c2 = Clause("Parent*", {"w", "a"}, {"w", "a"}, 0);
    REQUIRE(getSize(c2, declarations) == 3);

    Clause c3 = Clause("Uses", {"3", "_"}, {}, 1);
    REQUIRE(getSize(c3, declarations) == 8);

    Clause c4 = Clause("Modifies", {"p", "v"}, {"p", "v"}, 0);
    REQUIRE(getSize(c4, declarations) == 3);

    Clause c5 = Clause("", {"\"x0\"", "\"x0\""}, {}, 2);
    REQUIRE(getSize(c5, declarations) == 0);

    Clause c6 = Clause("", {"v.varName", "r.varName"}, {"v", "r"}, 0);
    REQUIRE(getSize(c6, declarations) == 7);

    Clause c7 = Clause("a", {"_", "_\"10\"_"}, {"a"}, 1);
    REQUIRE(getSize(c7, declarations) == 4);

    Clause c8 = Clause("ifs", {"v", "_", "_"}, {"ifs", "v"}, 0);
    REQUIRE(getSize(c8, declarations) == 0);

    Clause c9 = Clause("NextBip*", {"3", "n"}, {"n"}, 1);
    REQUIRE(getSize(c9, declarations) == 31);

    Clause c10 = Clause("AffectsBip", {"5", "5"}, {}, 2);
    REQUIRE(getSize(c10, declarations) == 5);
}
