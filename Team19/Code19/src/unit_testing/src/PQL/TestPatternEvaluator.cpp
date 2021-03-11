#include "PQL/PatternEvaluator.h"
#include "SP/DesignExtractor.h"
#include "catch.hpp"

using namespace std;

class StmtNodeStub : public ast::Stmt {
public:
	StmtNodeStub(int index) : ast::Stmt(new sp::Token(), index) {};
};

void setupAssignStmts() {
	DesignExtractor::signalReset();

	PKB::varTable = new VarTable();
	PKB::varTable->storeVarName("x"); // 0
	PKB::varTable->storeVarName("y"); // 1
	PKB::varTable->storeVarName("z"); // 2
	PKB::varTable->storeVarName("t"); // 3
	PKB::varTable->storeVarName("v"); // 4
	PKB::varTable->storeVarName("cenX"); // 5
	PKB::varTable->storeVarName("w"); // 6
	PKB::varTable->storeVarName("urgh"); // 7
	PKB::varTable->storeVarName("we"); // 8

	// "x = v + x * y + z * t;"
	std::vector<sp::Token*> stubTokens1{
			new sp::Token(sp::Token::TokenType::NAME, "x"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::NAME, "v"),
			new sp::Token(sp::Token::TokenType::PLUS, "+"),
			new sp::Token(sp::Token::TokenType::NAME, "x"),
			new sp::Token(sp::Token::TokenType::TIMES, "*"),
			new sp::Token(sp::Token::TokenType::NAME, "y"),
			new sp::Token(sp::Token::TokenType::PLUS, "+"),
			new sp::Token(sp::Token::TokenType::NAME, "z"),
			new sp::Token(sp::Token::TokenType::TIMES, "*"),
			new sp::Token(sp::Token::TokenType::NAME, "t"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l1 = new LexerStub(stubTokens1);
	Parser p1 = Parser(l1);
	ast::AssignStmt* stmt1 = p1.parseAssignStmt();

	// "cenX = 3 * 4 + (2 - 4);"
	std::vector<sp::Token*> stubTokens2{
			new sp::Token(sp::Token::TokenType::NAME, "cenX"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::CONST, "3"),
			new sp::Token(sp::Token::TokenType::TIMES, "*"),
			new sp::Token(sp::Token::TokenType::CONST, "4"),
			new sp::Token(sp::Token::TokenType::PLUS, "+"),
			new sp::Token(sp::Token::TokenType::LPAREN, "("),
			new sp::Token(sp::Token::TokenType::CONST, "2"),
			new sp::Token(sp::Token::TokenType::MINUS, "-"),
			new sp::Token(sp::Token::TokenType::CONST, "4"),
			new sp::Token(sp::Token::TokenType::RPAREN, ")"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l2 = new LexerStub(stubTokens2);
	Parser p2 = Parser(l2);
	ast::AssignStmt* stmt2 = p2.parseAssignStmt();

	// "urgh = 3 * ((2 - 4) / w);"
	std::vector<sp::Token*> stubTokens3{
			new sp::Token(sp::Token::TokenType::NAME, "urgh"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::CONST, "3"),
			new sp::Token(sp::Token::TokenType::TIMES, "*"),
			new sp::Token(sp::Token::TokenType::LPAREN, "("),
			new sp::Token(sp::Token::TokenType::LPAREN, "("),
			new sp::Token(sp::Token::TokenType::CONST, "2"),
			new sp::Token(sp::Token::TokenType::MINUS, "-"),
			new sp::Token(sp::Token::TokenType::CONST, "4"),
			new sp::Token(sp::Token::TokenType::RPAREN, ")"),
			new sp::Token(sp::Token::TokenType::DIV, "/"),
			new sp::Token(sp::Token::TokenType::NAME, "w"),
			new sp::Token(sp::Token::TokenType::RPAREN, ")"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l3 = new LexerStub(stubTokens3);
	Parser p3 = Parser(l3);
	ast::AssignStmt* stmt3 = p3.parseAssignStmt();

	// "x = y;"
	std::vector<sp::Token*> stubTokens4{
			new sp::Token(sp::Token::TokenType::NAME, "x"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::NAME, "y"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l4 = new LexerStub(stubTokens4);
	Parser p4 = Parser(l4);
	ast::AssignStmt* stmt4 = p4.parseAssignStmt();

	// "t = y;"
	std::vector<sp::Token*> stubTokens5{
			new sp::Token(sp::Token::TokenType::NAME, "t"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::NAME, "y"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l5 = new LexerStub(stubTokens5);
	Parser p5 = Parser(l5);
	ast::AssignStmt* stmt5 = p5.parseAssignStmt();

	// "we = 1000;"
	std::vector<sp::Token*> stubTokens6{
			new sp::Token(sp::Token::TokenType::NAME, "we"),
			new sp::Token(sp::Token::TokenType::ASSIGN, "="),
			new sp::Token(sp::Token::TokenType::NAME, "1000"),
			new sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
	};
	auto l6 = new LexerStub(stubTokens6);
	Parser p6 = Parser(l6);
	ast::AssignStmt* stmt6 = p6.parseAssignStmt();


	DesignExtractor::storeNewAssignment(1, "x", stmt1);
	DesignExtractor::storeNewAssignment(2, "cenX", stmt2);
	DesignExtractor::storeNewAssignment(3, "urgh", stmt3);
	DesignExtractor::storeNewAssignment(7, "x", stmt4);
	DesignExtractor::storeNewAssignment(21, "t", stmt5);
	DesignExtractor::storeNewAssignment(22, "t", stmt6);
}

void setUpIfWhileStmts() {
	PKB::varTable = new VarTable();
	PKB::varTable->storeVarName("x"); // 0
	PKB::varTable->storeVarName("y"); // 1
	PKB::varTable->storeVarName("z"); // 2
	PKB::varTable->storeVarName("t"); // 3
	PKB::varTable->storeVarName("v"); // 4
	PKB::varTable->storeVarName("cenX"); // 5
	PKB::varTable->storeVarName("w"); // 6
	PKB::varTable->storeVarName("urgh"); // 7
	PKB::varTable->storeVarName("we"); // 8

	PKB::stmtTable = new StmtTable();
	ast::Stmt* stmtNodeStub = new StmtNodeStub(0);
	PKB::stmtTable->storeStmt(1, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(2, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(3, stmtNodeStub, WHILE_);
	PKB::stmtTable->storeStmt(4, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(6, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(9, stmtNodeStub, WHILE_);
	PKB::stmtTable->storeStmt(10, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(13, stmtNodeStub, IF_);
	PKB::stmtTable->storeStmt(16, stmtNodeStub, WHILE_);
	PKB::stmtTable->storeStmt(17, stmtNodeStub, WHILE_);

	PKB::stmtTable->storeIfPattern(1, 0);
	PKB::stmtTable->storeIfPattern(1, 2);
	PKB::stmtTable->storeIfPattern(2, 1);
	PKB::stmtTable->storeWhilePattern(3, 1);
	PKB::stmtTable->storeWhilePattern(3, 2);
	PKB::stmtTable->storeIfPattern(4, 0);
	PKB::stmtTable->storeIfPattern(6, 4);
	PKB::stmtTable->storeWhilePattern(9, 6);
	PKB::stmtTable->storeIfPattern(10, 3);
	PKB::stmtTable->storeIfPattern(13, 7);
	PKB::stmtTable->storeWhilePattern(16, 7);
	PKB::stmtTable->storeWhilePattern(17, 8);
}

TEST_CASE("PatternEvaluator evaluate stmt Name Underscore") {
	setupAssignStmts();

	// a("x", _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"x\"", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{ 1, 7 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);
	
	// a("y", _)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"y\"", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvaluator evaluate stmt Name Name") {
	setupAssignStmts();

	// a("x", "y")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"x\"", "\"y\""}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{ 7 };
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	// a("x", "z")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"x\"", "\"z\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}


TEST_CASE("PatternEvaluator evaluate stmt Name Expression") {
	setupAssignStmts();

	// a("urgh", "3 * ((2 - 4) / w)")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"urgh\"", "\"3 * ((2 - 4) / w)\""}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{ 3 };
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	// a("x", "x * y")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"x\"", "\"x * y\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvaluator evaluate stmt Name ExpressionWithUnderscore") {
	setupAssignStmts();

	// a("urgh", _"3 * ((2 - 4) / w)"_)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"urgh\"", "_\"3 * ((2 - 4) / w)\"_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{ 3 }; 
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	
	// a("cenX", _"2 - 4"_)
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"cenX\"", "_\"2 - 4\"_"}), tempResults3);
	unordered_set<int> actual3(tempResults3["a"].begin(), tempResults3["a"].end());
	unordered_set<int> expected3{ 2 };
	REQUIRE(actual3 == expected3);
	REQUIRE(result3);

	// a("t", _"y"_)
	unordered_map<string, vector<int>> tempResults4;
	bool result4 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"t\"", "_\"y\"_"}), tempResults4);
	unordered_set<int> actual4(tempResults4["a"].begin(), tempResults4["a"].end());
	unordered_set<int> expected4{ 21 };
	REQUIRE(actual4 == expected4);
	REQUIRE(result4);

	// a("x", "v + z * t")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"\"x\"", "\"v + z * t\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvaluator evaluate Variable Underscore (assignment stmts exists)") {
	setupAssignStmts();

	// a(x, _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"x", VARIABLE_} }, Clause("a", vector<string>{"x", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> actual2(tempResults1["x"].begin(), tempResults1["x"].end());
	unordered_set<int> expected1{ 1, 2, 3, 7, 21, 22};
	unordered_set<int> expected2{ 0, 5, 7, 3 , 8};
	REQUIRE(actual1 == expected1);
	REQUIRE(actual2 == expected2);
	REQUIRE(result1);
}

TEST_CASE("PatternEvaluator evaluate Variable Underscore (assignment stmts does not exist)") {
	DesignExtractor::signalReset();
	// a(x, _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"x", VARIABLE_} }, Clause("a", vector<string>{"x", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> actual2(tempResults1["x"].begin(), tempResults1["x"].end());
	unordered_set<int> expected1{};
	unordered_set<int> expected2{};
	REQUIRE(actual1 == expected1);
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvaluator evaluate stmt Variable Name") {
	setupAssignStmts();

	// a(v, "y")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "\"y\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> actual2(tempResults1["v"].begin(), tempResults1["v"].end());
	unordered_set<int> expected1{ 7, 21 };
	unordered_set<int> expected2{ 0, 3 };
	REQUIRE(actual1 == expected1);
	REQUIRE(actual2 == expected2);
	REQUIRE(result1);
	
	// a(v, "v")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "\"v\""}), tempResults2);
	unordered_set<int> actual3(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> actual4(tempResults2["v"].begin(), tempResults2["v"].end());
	unordered_set<int> expected3{};
	unordered_set<int> expected4{};
	REQUIRE(actual3 == expected3);
	REQUIRE(actual4 == expected4);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvaluator evaluate stmt Variable expression") {
	setupAssignStmts();

	// a(v, "3 * 4 + (2 - 4)")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "\"3 * 4 + (2 - 4)\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> actual2(tempResults1["v"].begin(), tempResults1["v"].end());
	unordered_set<int> expected1{ 2 };
	unordered_set<int> expected2{ 5 };
	REQUIRE(actual1 == expected1);
	REQUIRE(actual2 == expected2);
	REQUIRE(result1);

	// a(v, "3 * 4")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "\"3 * 4\""}), tempResults2);
	unordered_set<int> actual3(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> actual4(tempResults2["v"].begin(), tempResults2["v"].end());
	unordered_set<int> expected3{};
	unordered_set<int> expected4{};
	REQUIRE(actual3 == expected3);
	REQUIRE(actual4 == expected4);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvaluator evaluate stmt Variable ExpressionWithUnderscore") {
	setupAssignStmts();

	// a(v, _"3 * 4 + (2 - 4)"_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "_\"3 * 4 + (2 - 4)\"_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> actual2(tempResults1["v"].begin(), tempResults1["v"].end());
	unordered_set<int> expected1{ 2 };
	unordered_set<int> expected2{ 5 };
	REQUIRE(actual1 == expected1);
	REQUIRE(actual2 == expected2);
	REQUIRE(result1);

	// a(v, _"3 * 4"_)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "_\"3 * 4\"_"}), tempResults2);
	unordered_set<int> actual3(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> actual4(tempResults2["v"].begin(), tempResults2["v"].end());
	unordered_set<int> expected3{ 2 };
	unordered_set<int> expected4{ 5 };
	REQUIRE(actual3 == expected3);
	REQUIRE(actual4 == expected4);
	REQUIRE(result2);

	// a(v, _"4 / w"_)
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"a", VARIABLE_}, {"v", VARIABLE_} }, Clause("a", vector<string>{"v", "_\"4 / w\"_"}), tempResults3);
	unordered_set<int> actual5(tempResults3["a"].begin(), tempResults3["a"].end());
	unordered_set<int> actual6(tempResults3["v"].begin(), tempResults3["v"].end());
	unordered_set<int> expected5{};
	unordered_set<int> expected6{};
	REQUIRE(actual5 == expected5);
	REQUIRE(actual6 == expected6);
	REQUIRE_FALSE(result3);

	// a(v, _"y"_)
	unordered_map<string, vector<int>> tempResults4;
	bool result5 = PatternEvaluator::evaluate({ {"a", VARIABLE_},  {"v", VARIABLE_}}, Clause("a", vector<string>{"v", "_\"y\"_"}), tempResults4);
	unordered_set<int> actual7(tempResults4["a"].begin(), tempResults4["a"].end());
	unordered_set<int> actual8(tempResults4["v"].begin(), tempResults4["v"].end());
	unordered_set<int> expected7{ 1, 7, 21 };
	unordered_set<int> expected8{ 0, 3 };
	REQUIRE(actual7 == expected7);
	REQUIRE(actual8 == expected8);
	REQUIRE(result5);
}

TEST_CASE("PatternEvaluator evaluate stmt Underscore Underscore (assignment stmt exists)") {
	setupAssignStmts();

	// a(_, _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_}}, Clause("a", vector<string>{"_", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{ 1, 2, 3, 7, 21, 22};
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);
}

TEST_CASE("PatternEvaluator evaluate stmt Underscore Underscore (assignment stmt does not exist)") {
	DesignExtractor::signalReset();

	// a(_, _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvaluator evaluate stmt Underscore Name") {
	setupAssignStmts();

	// a(_, "y")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "\"y\""}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{ 7, 21 };
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	// a(_, "z")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "\"z\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvaluator evaluate stmt Underscore expression") {
	setupAssignStmts();

	// a(_, "y")
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "\"y\""}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{ 7, 21 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// a(_, "3 * 4")
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_}}, Clause("a", vector<string>{"_", "\"3 * 4\""}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);

	// a(_, "3 * ((2 - 4) / w)")
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "\"3 * ((2 - 4) / w)\""}), tempResults3);
	unordered_set<int> actual3(tempResults3["a"].begin(), tempResults3["a"].end());
	unordered_set<int> expected3{ 3 };
	REQUIRE(actual3 == expected3);
	REQUIRE(result3);
}

TEST_CASE("PatternEvaluator evaluate stmt Underscore ExpressionWithUnderscore") {
	setupAssignStmts();

	// a(_, _"y"_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "_\"y\"_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{ 1, 7, 21 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// a(_, _"3 / w"_)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "_\"3 / w\"_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["a"].begin(), tempResults2["a"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);

	// a(_, _"(2 - 4) / w"_)
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "_\"(2 - 4) / w\"_"}), tempResults3);
	unordered_set<int> actual3(tempResults3["a"].begin(), tempResults3["a"].end());
	unordered_set<int> expected3{ 3 };
	REQUIRE(actual3 == expected3);
	REQUIRE(result3);
}

TEST_CASE("PatternEvaluator evaluate stmt underscore substring (edge case)") {
	setupAssignStmts();

	// a(_, _"1"_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"a", VARIABLE_} }, Clause("a", vector<string>{"_", "_\"1\"_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["a"].begin(), tempResults1["a"].end());
	unordered_set<int> expected1{};
	REQUIRE(actual1 == expected1);
	REQUIRE_FALSE(result1);
}

TEST_CASE("PatternEvalautor evalaute if stmt synonym underscore underscore") {
	setUpIfWhileStmts();

	// ifs(v,_,_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"v", VARIABLE_}, {"ifs", IF_} }, Clause("ifs", vector<string>{"v", "_", "_"}), tempResults1);
	unordered_set<int> actual11(tempResults1["v"].begin(), tempResults1["v"].end());
	unordered_set<int> actual12(tempResults1["ifs"].begin(), tempResults1["ifs"].end());
	unordered_set<int> expected11{0, 1, 2, 3, 4, 7};
	unordered_set<int> expected12{ 1, 2, 4, 6, 10, 13 };
	REQUIRE(actual11 == expected11);
	REQUIRE(actual12 == expected12);
	REQUIRE(result1);

	// ifs(v,_,_)
	DesignExtractor::signalReset();
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"v", VARIABLE_}, {"ifs", IF_} }, Clause("ifs", vector<string>{"v", "_", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["ifs"].begin(), tempResults2["ifs"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvalautor evalaute if stmt underscore underscore underscore") {
	setUpIfWhileStmts();

	// ifs(_,_,_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"ifs", IF_} }, Clause("ifs", vector<string>{"_", "_", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["ifs"].begin(), tempResults1["ifs"].end());
	unordered_set<int> expected1{ 1, 2, 4, 6, 10, 13 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// ifs(_,_,_)
	DesignExtractor::signalReset();
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"ifs", IF_} }, Clause("ifs", vector<string>{"_", "_", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["ifs"].begin(), tempResults2["ifs"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvalautor evalaute if stmt name underscore underscore") {
	setUpIfWhileStmts();

	// ifs("x",_,_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"ifs", IF_} }, Clause("ifs", vector<string>{"\"x\"", "_", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["ifs"].begin(), tempResults1["ifs"].end());
	unordered_set<int> expected1{ 1, 4 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// ifs("urgh",_,_)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"ifs", IF_} }, Clause("ifs", vector<string>{"\"urgh\"", "_", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["ifs"].begin(), tempResults2["ifs"].end());
	unordered_set<int> expected2{ 13 };
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	// ifs("cenX",_,_)
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"ifs", IF_} }, Clause("ifs", vector<string>{"\"cenX\"", "_", "_"}), tempResults3);
	unordered_set<int> actual3(tempResults3["ifs"].begin(), tempResults3["ifs"].end());
	unordered_set<int> expected3{};
	REQUIRE(actual3 == expected3);
	REQUIRE_FALSE(result3);
}

TEST_CASE("PatternEvalautor evalaute while stmt synonym underscore") {
	setUpIfWhileStmts();

	// w(v,_)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"v", VARIABLE_}, {"w", WHILE_} }, Clause("w", vector<string>{"v", "_"}), tempResults1);
	unordered_set<int> actual11(tempResults1["v"].begin(), tempResults1["v"].end());
	unordered_set<int> actual12(tempResults1["w"].begin(), tempResults1["w"].end());
	unordered_set<int> expected11{ 1, 2, 6, 7, 8 };
	unordered_set<int> expected12{ 3, 9, 16, 17 };
	REQUIRE(actual11 == expected11);
	REQUIRE(actual12 == expected12);
	REQUIRE(result1);

	// w(v,_)
	DesignExtractor::signalReset();
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"v", VARIABLE_}, {"w",	WHILE_} }, Clause("w", vector<string>{"v", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["w"].begin(), tempResults2["w"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvalautor evalaute while stmt underscore underscore") {
	setUpIfWhileStmts();

	// w(_, _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"_", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
	unordered_set<int> expected1{ 3, 9, 16, 17 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// w(_, _)
	DesignExtractor::signalReset();
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"_", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["w"].begin(), tempResults2["w"].end());
	unordered_set<int> expected2{};
	REQUIRE(actual2 == expected2);
	REQUIRE_FALSE(result2);
}

TEST_CASE("PatternEvalautor evalaute while stmt name underscore") {
	setUpIfWhileStmts();

	// w("w", _)
	unordered_map<string, vector<int>> tempResults1;
	bool result1 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"\"w\"", "_"}), tempResults1);
	unordered_set<int> actual1(tempResults1["w"].begin(), tempResults1["w"].end());
	unordered_set<int> expected1{ 9 };
	REQUIRE(actual1 == expected1);
	REQUIRE(result1);

	// w("z",_)
	unordered_map<string, vector<int>> tempResults2;
	bool result2 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"\"z\"", "_"}), tempResults2);
	unordered_set<int> actual2(tempResults2["w"].begin(), tempResults2["w"].end());
	unordered_set<int> expected2{ 3 };
	REQUIRE(actual2 == expected2);
	REQUIRE(result2);

	// w("y",_)
	unordered_map<string, vector<int>> tempResults3;
	bool result3 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"\"y\"", "_"}), tempResults3);
	unordered_set<int> actual3(tempResults3["w"].begin(), tempResults3["w"].end());
	unordered_set<int> expected3{ 3 };
	REQUIRE(actual3 == expected3);
	REQUIRE(result3);

	// w("x", _)
	unordered_map<string, vector<int>> tempResults4;
	bool result4 = PatternEvaluator::evaluate({ {"w", WHILE_} }, Clause("w", vector<string>{"\"x\"", "_"}), tempResults4);
	unordered_set<int> actual4(tempResults4["w"].begin(), tempResults4["w"].end());
	unordered_set<int> expected4{};
	REQUIRE(actual4 == expected4);
	REQUIRE_FALSE(result4);
}
