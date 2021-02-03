#include <string>
#include <vector>
#include "SP/Lexer.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("tokenise Test") {
	//valid string
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

	string test1 = "procedure q {z = x + 12;}";
	vector<sp::Token> actual;
	REQUIRE(Lexer::tokenise(test1, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected[i].getType() == actual[i].getType());
	}

	//various types of whitespace
	string test2 = "procedure q {z = \nx + \t       12;}";
	actual.clear();
	REQUIRE(Lexer::tokenise(test2, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected[i].getType() == actual[i].getType());
	}

	//empty string
	actual.clear();
	REQUIRE_FALSE(Lexer::tokenise("", actual));
	REQUIRE(actual.size() == 0);

	//invalid name
	string test3 = "procedure q {z = 123x + 12;}";
	actual.clear();
	vector<sp::Token> expected3{
		sp::Token(sp::Token::TokenType::PROC, "procedure"),
		sp::Token(sp::Token::TokenType::NAME, "q"),
		sp::Token(sp::Token::TokenType::LBRACE, "{"),
		sp::Token(sp::Token::TokenType::NAME, "z"),
		sp::Token(sp::Token::TokenType::ASSIGN, "="),
		sp::Token(sp::Token::TokenType::ERROR, "123x")
	};
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

}


