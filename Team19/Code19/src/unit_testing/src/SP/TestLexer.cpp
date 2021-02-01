#include <string>
#include <vector>
#include "SP/Lexer.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("tokenise Test") {
	//valid string
	vector<Token> expected{
		Token(Token::TokenType::PROC, "procedure"),
		Token(Token::TokenType::NAME, "q"),
		Token(Token::TokenType::LBRACE, "{"),
		Token(Token::TokenType::NAME, "z"),
		Token(Token::TokenType::ASSIGN, "="),
		Token(Token::TokenType::NAME, "x"),
		Token(Token::TokenType::PLUS, "+"),
		Token(Token::TokenType::CONST, "12"),
		Token(Token::TokenType::SEMICOLON, ";"),
		Token(Token::TokenType::RBRACE, "}"),
		Token(Token::TokenType::EOFF, "EOF"),
	};

	string test1 = "procedure q {z = x + 12;}";
	vector<Token> actual;
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
	vector<Token> expected3{
		Token(Token::TokenType::PROC, "procedure"),
		Token(Token::TokenType::NAME, "q"),
		Token(Token::TokenType::LBRACE, "{"),
		Token(Token::TokenType::NAME, "z"),
		Token(Token::TokenType::ASSIGN, "="),
		Token(Token::TokenType::ERROR, "123x")
	};
	REQUIRE_FALSE(Lexer::tokenise(test3, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected3[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected3[i].getType() == actual[i].getType());
	}

	//invalid operators eg &, |
	string test4 = "procedure q {z = 12&12;}";
	actual.clear();
	vector<Token> expected4{
		Token(Token::TokenType::PROC, "procedure"),
		Token(Token::TokenType::NAME, "q"),
		Token(Token::TokenType::LBRACE, "{"),
		Token(Token::TokenType::NAME, "z"),
		Token(Token::TokenType::ASSIGN, "="),
		Token(Token::TokenType::CONST, "12"),
		Token(Token::TokenType::ERROR, "&")
	};
	REQUIRE_FALSE(Lexer::tokenise(test4, actual));
	for (int i = 0; i < actual.size(); i++) {
		REQUIRE(expected4[i].getLiteral() == actual[i].getLiteral());
		REQUIRE(expected4[i].getType() == actual[i].getType());
	}

}


