#include "Lexer.h"

#include <algorithm> // for all_of function

//from pql/Utility
bool checkName(string s) {
	bool isValid = false;
	if (isalpha(s[0])) {
		isValid = true;
		for (int i = 1; i <= s.length() - 1; i++)
		{
			isValid = (isalnum(s[i]));

			if (!isValid) {
				break;
			}
		}
	}
	return isValid;
}

//from pql/Utility
bool isNumber(string s) {
	return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

bool Lexer::tokenise(string str, vector<Token>& result) {
	
	if (str == "") {
		return false;
	}
	string::iterator it = str.begin();

	Token token;
	while (it != str.end()) {
		token = nextToken(it);
		if (token.getType() == Token::TokenType::ERROR) {
			return false;
		}
		result.push_back(token);
		it++;
	}
	result.push_back(Token(Token::TokenType::EOFF, "EOF"));
	return true;

}

Token Lexer::nextToken(string::iterator& it) {
	skipWhitespace(it);
	Token token;
	char next;
	switch (*it) {
		case '+':
			token = Token(Token::TokenType::PLUS, "+");
			break;
		case '-':
			token = Token(Token::TokenType::MINUS, "-");
			break;
		case '*':
			token = Token(Token::TokenType::TIMES, "*");
			break;
		case '/':
			token = Token(Token::TokenType::DIV, "/");
			break;
		case '%':
			token = Token(Token::TokenType::MOD, "%");
			break;
		case ';':
			token = Token(Token::TokenType::SEMICOLON, ";");
			break;
		case '(':
			token = Token(Token::TokenType::LPAREN, "(");
			break;
		case ')':
			token = Token(Token::TokenType::RPAREN, ")");
			break;
		case '{':
			token = Token(Token::TokenType::LBRACE, "{");
			break;
		case '}':
			token = Token(Token::TokenType::RBRACE, "}");
			break;
		case '=':
			next = *(it + 1);
			if (next == '=') {
				token = Token(Token::TokenType::EQ, "==");
				it++;
			} else {
				token = Token(Token::TokenType::ASSIGN, "=");
			}
			break;
		case '!':
			next = *(it + 1);
			if (next == '=') {
				token = Token(Token::TokenType::NEQ, "!=");
				it++;
			} else {
				token = Token(Token::TokenType::NOT, "!");
			}
			break;
		case '<':
			next = *(it + 1);
			if (next == '=') {
				token = Token(Token::TokenType::LTE, "<=");
				it++;
			} else {
				token = Token(Token::TokenType::LT, "<");
			}
			break;
		case '>':
			next = *(it + 1);
			if (next == '=') {
				token = Token(Token::TokenType::GTE, ">=");
				it++;
			} else {
				token = Token(Token::TokenType::GT, ">");
			}
			break;
		case '&':
			next = *(it + 1);
			if (next == '&') {
				token = Token(Token::TokenType::AND, "&&");
				it++;
			} else {
				token = Token(Token::TokenType::ERROR, "&");
			}
			break;
		case '|':
			next = *(it + 1);
			if (next == '|') {
				token = Token(Token::TokenType::OR, "||");
				it++;
			}
			else {
				token = Token(Token::TokenType::ERROR, "|");
			}
			break;
		default:
			token = makeNameConstKeyToken(it);
			break;
	}
	return token;
}

void Lexer::skipWhitespace(string::iterator& it) {
	string whitespace = " \t\n\r\f\v";
	while (whitespace.find(*it) < whitespace.length()) {
		it++;
	}
}

Token Lexer::makeNameConstKeyToken(string::iterator& it) {
	string literal = "";
	while (isdigit(*it) || isalpha(*it)) {
		literal += string(1, *it);
		it++;
	}
	
	it--;
	if (literal == "procedure") {
		return Token(Token::TokenType::PROC, literal);
	} else if (literal == "read") {
		return Token(Token::TokenType::READ, literal);
	} else if (literal == "print") {
		return Token(Token::TokenType::PRINT, literal);
	} else if (literal == "call") {
		return Token(Token::TokenType::CALL, literal);
	} else if (literal == "while") {
		return Token(Token::TokenType::WHILE, literal);
	} else if (literal == "if") {
		return Token(Token::TokenType::IF, literal);
	} else if (literal == "then") {
		return Token(Token::TokenType::THEN, literal);
	} else if (literal == "else") {
		return Token(Token::TokenType::ELSE, literal);
	} else if (checkName(literal)) {
		return Token(Token::TokenType::NAME, literal);
	} else if (isNumber(literal)) {
		return Token(Token::TokenType::CONST, literal);
	} else {
		return Token(Token::TokenType::ERROR, literal);
	}
	
}
