#include "Lexer.h"
#include "../Utility.h"

//#include <algorithm> // for all_of function
//
//from pql/Utility
//bool checkName(string s) {
//	bool isValid = false;
//	if (isalpha(s[0])) {
//		isValid = true;
//		for (int i = 1; i <= s.length() - 1; i++)
//		{
//			isValid = (isalnum(s[i]));
//
//			if (!isValid) {
//				break;
//			}
//		}
//	}
//	return isValid;
//}
//
//from pql/Utility
//bool checkInteger(string s) {
//	return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
//}

bool Lexer::tokenise(string str, vector<sp::Token>& result) {
	
	if (str == "") {
		return false;
	}
	string::iterator it = str.begin();

	sp::Token token;
	while (it != str.end()) {
		token = nextToken(it);
		if (token.getType() == sp::Token::TokenType::ERROR) {
			return false;
		}
		result.push_back(token);
		it++;
	}
	result.push_back(sp::Token(sp::Token::TokenType::EOFF, "EOF"));
	return true;

}

sp::Token Lexer::nextToken(string::iterator& it) {
	skipWhitespace(it);
	sp::Token token;
	char next;
	switch (*it) {
		case '+':
			token = sp::Token(sp::Token::TokenType::PLUS, "+");
			break;
		case '-':
			token = sp::Token(sp::Token::TokenType::MINUS, "-");
			break;
		case '*':
			token = sp::Token(sp::Token::TokenType::TIMES, "*");
			break;
		case '/':
			token = sp::Token(sp::Token::TokenType::DIV, "/");
			break;
		case '%':
			token = sp::Token(sp::Token::TokenType::MOD, "%");
			break;
		case ';':
			token = sp::Token(sp::Token::TokenType::SEMICOLON, ";");
			break;
		case '(':
			token = sp::Token(sp::Token::TokenType::LPAREN, "(");
			break;
		case ')':
			token = sp::Token(sp::Token::TokenType::RPAREN, ")");
			break;
		case '{':
			token = sp::Token(sp::Token::TokenType::LBRACE, "{");
			break;
		case '}':
			token = sp::Token(sp::Token::TokenType::RBRACE, "}");
			break;
		case '=':
			next = *(it + 1);
			if (next == '=') {
				token = sp::Token(sp::Token::TokenType::EQ, "==");
				it++;
			} else {
				token = sp::Token(sp::Token::TokenType::ASSIGN, "=");
			}
			break;
		case '!':
			next = *(it + 1);
			if (next == '=') {
				token = sp::Token(sp::Token::TokenType::NEQ, "!=");
				it++;
			} else {
				token = sp::Token(sp::Token::TokenType::NOT, "!");
			}
			break;
		case '<':
			next = *(it + 1);
			if (next == '=') {
				token = sp::Token(sp::Token::TokenType::LTE, "<=");
				it++;
			} else {
				token = sp::Token(sp::Token::TokenType::LT, "<");
			}
			break;
		case '>':
			next = *(it + 1);
			if (next == '=') {
				token = sp::Token(sp::Token::TokenType::GTE, ">=");
				it++;
			} else {
				token = sp::Token(sp::Token::TokenType::GT, ">");
			}
			break;
		case '&':
			next = *(it + 1);
			if (next == '&') {
				token = sp::Token(sp::Token::TokenType::AND, "&&");
				it++;
			} else {
				token = sp::Token(sp::Token::TokenType::ERROR, "&");
			}
			break;
		case '|':
			next = *(it + 1);
			if (next == '|') {
				token = sp::Token(sp::Token::TokenType::OR, "||");
				it++;
			}
			else {
				token = sp::Token(sp::Token::TokenType::ERROR, "|");
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

sp::Token Lexer::makeNameConstKeyToken(string::iterator& it) {
	string literal = "";
	while (isdigit(*it) || isalpha(*it)) {
		literal += string(1, *it);
		it++;
	}
	
	it--;
	if (literal == "procedure") {
		return sp::Token(sp::Token::TokenType::PROC, literal);
	} else if (literal == "read") {
		return sp::Token(sp::Token::TokenType::READ, literal);
	} else if (literal == "print") {
		return sp::Token(sp::Token::TokenType::PRINT, literal);
	} else if (literal == "call") {
		return sp::Token(sp::Token::TokenType::CALL, literal);
	} else if (literal == "while") {
		return sp::Token(sp::Token::TokenType::WHILE, literal);
	} else if (literal == "if") {
		return sp::Token(sp::Token::TokenType::IF, literal);
	} else if (literal == "then") {
		return sp::Token(sp::Token::TokenType::THEN, literal);
	} else if (literal == "else") {
		return sp::Token(sp::Token::TokenType::ELSE, literal);
	} else if (checkName(literal)) {
		return sp::Token(sp::Token::TokenType::NAME, literal);
	} else if (checkInteger(literal)) {
		return sp::Token(sp::Token::TokenType::CONST, literal);
	} else {
		return sp::Token(sp::Token::TokenType::ERROR, literal);
	}
	
}
