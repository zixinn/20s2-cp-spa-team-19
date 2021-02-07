#pragma once

#include <string>

using namespace std;

namespace sp {
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
			SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE,  //delimiters
			PROCNAME, CONSTNAME			// for parser use only
		};

		//default constructor for initialising
		Token();
		~Token();

		Token(TokenType type, string literal);

		TokenType getType();
		string getLiteral();
		bool compare(Token* other);

	private:
		TokenType type;
		string literal;
	};
	struct tokentype_hash { //hash<Token::TokenType> {
		size_t operator() (Token::TokenType const& x) const { return std::hash<int>()(int(x));  }
	};
}
