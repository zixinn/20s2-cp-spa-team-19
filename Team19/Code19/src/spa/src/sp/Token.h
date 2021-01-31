#pragma once

#include <string>

using namespace std;

class Token {
public:
	enum class TokenType {
		ERROR, EOFF,
		PROC, READ, PRINT, CALL, WHILE, IF, THEN, ELSE, ASSIGN,  //keywords
		NAME, CONST,   //identifiers
		PLUS, MINUS, TIMES, DIV, MOD,  //operators
		AND, OR,  //infix cond
		EQ, NEQ, GT, GTE, LT, LTE,  //rel expr
		NOT,  //prefix cond
		SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE  //delimiters
	};

	Token(TokenType type, string literal);
	
	TokenType getType();
	string getLiteral();
	bool compare(Token* other);

private:
	TokenType type;
	string literal;
	
};