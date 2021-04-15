#pragma once

#include <vector>

#include "Token.h"

using namespace std;

// Turns SIMPLE source strings into Tokens
class Lexer {
public:
	static bool tokenise(STRING str, vector<sp::Token>& result);

private:
	static sp::Token nextToken(STRING::iterator& it, STRING::iterator end);
	//Returns true if ptr points to a whitespace at the end, else false
	static bool skipWhitespace(STRING::iterator& it, STRING::iterator end);
	static sp::Token makeNameConstKeyToken(STRING::iterator& it, STRING::iterator end);
};
