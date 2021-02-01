#pragma once

#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Lexer {
public:
	static bool tokenise(string str, vector<Token>& result);

private:
	static Token nextToken(string::iterator& it);
	static void skipWhitespace(string::iterator& it);
	static Token makeNameConstKeyToken(string::iterator& it);
};