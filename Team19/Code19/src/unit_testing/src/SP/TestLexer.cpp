#include <string>
#include <vector>
#include "SP/Lexer.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("Tokenise - Basic Test") {
	vector<sp::Token> expected{
		sp::Token(sp::Token::TokenType::PROC, "procedure"),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::NAME, "z"),
		sp::Token(sp::Token::TokenType::ASSIGN, "="),
		sp::Token(sp::Token::TokenType::NAME, "x"),
		sp::Token(sp::Token::TokenType::PLUS, "+"),
		sp::Token(sp::Token::TokenType::CONST, "12"),
		sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
		sp::Token(sp::Token::TokenType::IF, "if"),
		sp::Token(sp::Token::TokenType::LPAREN, "("),
		sp::Token(sp::Token::TokenType::NAME, "a"),
		sp::Token(sp::Token::TokenType::EQ, "=="),
		sp::Token(sp::Token::TokenType::NAME, "b"),
		sp::Token(sp::Token::TokenType::RPAREN, ")"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::RBRACE, "}"),
		sp::Token(sp::Token::TokenType::RBRACE, "}"),
		sp::Token(sp::Token::TokenType::EOFF, "EOF"),
	};

	//valid
	string test = "procedure q {z = x + 12; if (a == b) {}}";
	vector<sp::Token> actual;
	REQUIRE(Lexer::tokenise(test, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected[i].getType() == actual[i].getType());
	}

	//empty string
	actual.clear();
	REQUIRE_FALSE(Lexer::tokenise("", actual));
	REQUIRE(actual.size() == 0);
}

TEST_CASE("Tokenise - Whitespace Test") {
	vector<sp::Token> expected{
		sp::Token(sp::Token::TokenType::PROC, "procedure"),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::NAME, "z"),
		sp::Token(sp::Token::TokenType::ASSIGN, "="),
		sp::Token(sp::Token::TokenType::NAME, "x"),
		sp::Token(sp::Token::TokenType::PLUS, "+"),
		sp::Token(sp::Token::TokenType::CONST, "12"),
		sp::Token(sp::Token::TokenType::SEMICOLON, ";"),
		sp::Token(sp::Token::TokenType::RBRACE, "}"),
		sp::Token(sp::Token::TokenType::EOFF, "EOF"),
	};

	vector<sp::Token> actual;

	//various types of whitespace
	string test1 = "procedure q {z = \nx + \t       12;}";
	REQUIRE(Lexer::tokenise(test1, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected[i].getType() == actual[i].getType());
	}

	//trailing and leading whitespace
	string test2 = "    \n   \t  procedure q {z = x + 12;}   \n\t    \v";
	actual.clear();
	REQUIRE(Lexer::tokenise(test2, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected[i].getType() == actual[i].getType());
	}

}


TEST_CASE("Tokenise - End of String Test") {
	vector<sp::Token> actual;

	//end with name
	vector<sp::Token> expected1{
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::EQ, "=="),
		sp::Token(sp::Token::TokenType::NAME, "zz"),
		sp::Token(sp::Token::TokenType::EOFF, "EOF"),
	};

	string test1 = "q == zz";
	REQUIRE(Lexer::tokenise(test1, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected1[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected1[i].getType() == actual[i].getType());
	}

	//end with operator
	vector<sp::Token> expected2{
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::EQ, "=="),
		sp::Token(sp::Token::TokenType::EOFF, "EOF"),
	};
	string test2 = "q ==";
	actual.clear();
	REQUIRE(Lexer::tokenise(test2, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected2[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected2[i].getType() == actual[i].getType());
	}

	//end with delimiter
	vector<sp::Token> expected3{
		sp::Token(sp::Token::TokenType::LPAREN, "("),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LT, "<"),
		sp::Token(sp::Token::TokenType::NAME, "a"),
		sp::Token(sp::Token::TokenType::RPAREN, ")"),
		sp::Token(sp::Token::TokenType::EOFF, "EOF"),
	};
	string test3 = "(q < a)";
	actual.clear();
	REQUIRE(Lexer::tokenise(test3, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected3[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected3[i].getType() == actual[i].getType());
	}

	//invalid end
	vector<sp::Token> expected4{
		sp::Token(sp::Token::TokenType::LPAREN, "("),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LT, "<"),
	};

	string test4 = "(q < 12a";
	actual.clear();
	REQUIRE_FALSE(Lexer::tokenise(test4, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected4[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected4[i].getType() == actual[i].getType());
	}
}

TEST_CASE("Tokenise - Invalid Test") {

	//invalid name
	string test3 = "procedure q {z = 123x + 12;}";
	vector<sp::Token> expected3 {
		sp::Token(sp::Token::TokenType::PROC, "procedure"),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::NAME, "z"),
		sp::Token(sp::Token::TokenType::ASSIGN, "="),
		sp::Token(sp::Token::TokenType::ERROR, "123x")
	};

	vector<sp::Token> actual;
	REQUIRE_FALSE(Lexer::tokenise(test3, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected3[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected3[i].getType() == actual[i].getType());
	}

	//invalid operators eg &, |
	string test4 = "procedure q {z = 12&12;}";
	actual.clear();
	vector<sp::Token> expected4{
		sp::Token(sp::Token::TokenType::PROC, "procedure"),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::NAME, "z"),
		sp::Token(sp::Token::TokenType::ASSIGN, "="),
		sp::Token(sp::Token::TokenType::CONST, "12"),
		sp::Token(sp::Token::TokenType::ERROR, "&")
	};
	REQUIRE_FALSE(Lexer::tokenise(test4, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected4[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected4[i].getType() == actual[i].getType());
	}

	//invalid characters eg ", [, $
	vector<string> test5{
		"\"",
		"[a",
		"$rr"
	};
	for (int i = 0; i < test5.size(); i++) {
		actual.clear();
		REQUIRE_FALSE(Lexer::tokenise(test5[i], actual));
		REQUIRE(actual.size() == 0);
	}

}

