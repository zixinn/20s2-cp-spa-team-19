#pragma once

#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Lexer {
public:
	static bool tokenise(string str, vector<sp::Token>& result);

private:
	static sp::Token nextToken(string::iterator& it, string::iterator end);
	//Returns true if ptr points to a whitespace at the end, else false
	static bool skipWhitespace(string::iterator& it, string::iterator end);
	static sp::Token makeNameConstKeyToken(string::iterator& it, string::iterator end);
};
