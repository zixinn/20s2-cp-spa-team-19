#pragma once

#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Lexer {
public:
	static bool tokenise(string str, vector<sp::Token>& result);

private:
	static sp::Token nextToken(string::iterator& it);
	static void skipWhitespace(string::iterator& it);
	static sp::Token makeNameConstKeyToken(string::iterator& it);
};
