#include "PQL/QueryUtility.h"
#include "catch.hpp"

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
    unordered_map<string, string> declarations = {{"a", "assign"}, {"v", "variable"}};
    checkSynonymDeclared("a", declarations);
    REQUIRE(checkSynonymDeclared("a", declarations));
    REQUIRE_FALSE(checkSynonymDeclared("w", declarations));
}

TEST_CASE("getArgType") {
    unordered_map<string, string> declarations = {{"a", "assign"}, {"v", "variable"}};
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
    StmtNodeStub(int index): ast::Stmt(new sp::Token(), index){};
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
    REQUIRE(selectAll(PROCEDURE_) == vector<int>{0, 1});
    REQUIRE(selectAll(STMT_) == vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
    REQUIRE(selectAll(READ_) == vector<int>{2, 8});
    REQUIRE(selectAll(PRINT_) == vector<int>{3});
    REQUIRE(selectAll(ASSIGN_) == vector<int>{1, 7});
    REQUIRE(selectAll(CALL_) == vector<int>{4});
    REQUIRE(selectAll(WHILE_) == vector<int>{5});
    REQUIRE(selectAll(IF_) == vector<int>{6});
    REQUIRE(selectAll(VARIABLE_) == vector<int>{0, 1, 2});
    REQUIRE(selectAll(CONSTANT_) == vector<int>{2, 8});
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
