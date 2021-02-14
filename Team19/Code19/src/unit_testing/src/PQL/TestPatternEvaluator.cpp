#include "PQL/PatternEvaluator.h"
#include "SP/DesignExtractor.h"
#include "catch.hpp"

using namespace std;

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

	DesignExtractor::storeNewAssignment(1, "x", stmt1);
	DesignExtractor::storeNewAssignment(2, "cenX", stmt2);
	DesignExtractor::storeNewAssignment(3, "urgh", stmt3);
	DesignExtractor::storeNewAssignment(7, "x", stmt4);
	DesignExtractor::storeNewAssignment(21, "t", stmt5);
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
	unordered_set<int> expected1{ 1, 2, 3, 7, 21};
	unordered_set<int> expected2{ 0, 5, 7, 3 };
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
	unordered_set<int> expected1{ 1, 2, 3, 7, 21 };
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
