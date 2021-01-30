#include "Token.h"

Token::Token(TokenType type, string literal) {
	this->type = type;
	this->literal = literal;
}

Token::TokenType Token::getType() {
	return this->type;
}

string Token::getLiteral() {
	return this->literal;
}